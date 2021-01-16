/*
 * Copyright (C) 2021 Tusooa Zhu <tusooa@vista.aero>
 *
 * This file is part of libkazv.
 *
 * libkazv is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as
 * published by the Free Software Foundation, either version 3 of the
 * License, or (at your option) any later version.
 *
 * libkazv is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with libkazv.  If not, see <https://www.gnu.org/licenses/>.
 */

#include "encryption.hpp"

#include <debug.hpp>

namespace Kazv
{
    using namespace CryptoConstants;

    static json convertSignature(const ClientModel &m, std::string signature)
    {
        auto j = json::object();
        j[m.userId] = json::object();
        j[m.userId][ed25519 + ":" + m.deviceId] = signature;

        return j;
    }

    ClientResult updateClient(ClientModel m, UploadIdentityKeysAction)
    {
        if (! m.crypto) {
            kzo.client.warn() << "Client::crypto is invalid, ignoring it." << std::endl;
            return { std::move(m), lager::noop };
        }

        auto &crypto = m.crypto.value();

        auto keys =
            immer::map<std::string, std::string>{}
             .set(ed25519 + ":" + m.deviceId, crypto.ed25519IdentityKey())
             .set(curve25519 + ":" + m.deviceId, crypto.curve25519IdentityKey());

        DeviceKeys k {
            m.userId,
            m.deviceId,
            {olmAlgo, megOlmAlgo},
            keys,
            {} // signatures to be added soon
        };

        auto j = json(k);

        auto sig = crypto.sign(j);

        k.signatures = convertSignature(m, sig);

        auto job = m.job<UploadKeysJob>()
            .make(k)
            .withData(json{{"is", "identityKeys"}});

        kzo.client.dbg() << "Uploading identity keys" << std::endl;

        m.addJob(std::move(job));

        return { std::move(m), lager::noop };
    }

    ClientResult updateClient(ClientModel m, GenerateAndUploadOneTimeKeysAction)
    {
        if (! m.crypto) {
            kzo.client.warn() << "Client::crypto is invalid, ignoring it." << std::endl;
            return { std::move(m), lager::noop };
        }

        auto &crypto = m.crypto.value();

        // Keep half of max supported number of keys
        int numUploadedKeys = crypto.uploadedOneTimeKeysCount(signedCurve25519);
        int numKeysNeeded = crypto.maxNumberOfOneTimeKeys() / 2
            - numUploadedKeys;

        // Subtract the number of existing one-time keys, in case
        // the previous upload was not successful.
        int numKeysToGenerate = numKeysNeeded - crypto.numUnpublishedOneTimeKeys();

        kzo.client.dbg() << "Generating one-time keys..." << std::endl;
        kzo.client.dbg() << "Number needed: " << numKeysNeeded << std::endl;

        if (numKeysNeeded <= 0) { // we have enough already
            kzo.client.dbg() << "We have enough one-time keys. Ignoring this." << std::endl;
            return { std::move(m), lager::noop };
        }

        if (numKeysToGenerate > 0) {
            crypto.genOneTimeKeys(numKeysToGenerate);
        }
        kzo.client.dbg() << "Generating done." << std::endl;

        auto keys = crypto.unpublishedOneTimeKeys();

        auto cv25519Keys = keys.at(curve25519);
        immer::map<std::string, Variant> oneTimeKeys;
        for (auto [id, keyStr] : cv25519Keys.items()) {
            json keyObject = json::object();
            keyObject["key"] = keyStr;
            keyObject["signatures"] = convertSignature(m, crypto.sign(keyObject));

            oneTimeKeys = std::move(oneTimeKeys).set(signedCurve25519 + ":" + id, JsonWrap(keyObject));
        }

        auto job = m.job<UploadKeysJob>()
            .make(
                std::nullopt, // deviceKeys
                oneTimeKeys)
            .withData(json{{"is", "oneTimeKeys"}});

        kzo.client.dbg() << "Uploading one time keys" << std::endl;

        m.addJob(std::move(job));

        return { std::move(m), lager::noop };

    };

    ClientResult processResponse(ClientModel m, UploadKeysResponse r)
    {
        if (! m.crypto) {
            kzo.client.warn() << "Client::crypto is invalid, ignoring it." << std::endl;
            return { std::move(m), lager::noop };
        }

        auto &crypto = m.crypto.value();

        auto is = r.dataStr("is");

        if (is == "identityKeys") {
            if (! r.success()) {
                kzo.client.dbg() << "Uploading identity keys failed" << std::endl;
                m.addTrigger(UploadIdentityKeysFailed{r.errorCode(), r.errorMessage()});
                return { std::move(m), lager::noop };
            }

            kzo.client.dbg() << "Uploading identity keys successful" << std::endl;
            m.addTrigger(UploadIdentityKeysSuccessful{});

            m.identityKeysUploaded = true;
        } else {
            if (! r.success()) {
                kzo.client.dbg() << "Uploading one-time keys failed" << std::endl;
                m.addTrigger(UploadOneTimeKeysFailed{r.errorCode(), r.errorMessage()});
                return { std::move(m), lager::noop };
            }

            kzo.client.dbg() << "Uploading one-time keys successful" << std::endl;
            m.addTrigger(UploadOneTimeKeysSuccessful{});

            crypto.markOneTimeKeysAsPublished();
        }

        crypto.setUploadedOneTimeKeysCount(r.oneTimeKeyCounts());

        return { std::move(m), lager::noop };
    }

}
