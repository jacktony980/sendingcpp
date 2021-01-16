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
        j[ed25519 + ":" + m.deviceId] = signature;

        return j;
    }

    ClientResult updateClient(ClientModel m, UploadIdentityKeysAction a)
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

    ClientResult processResponse(ClientModel m, UploadKeysResponse r)
    {
        if (! m.crypto) {
            kzo.client.warn() << "Client::crypto is invalid, ignoring it." << std::endl;
            return { std::move(m), lager::noop };
        }

        if (! r.success()) {
            kzo.client.dbg() << "Uploading identity keys failed" << std::endl;
            m.addTrigger(UploadIdentityKeysFailed{r.errorCode(), r.errorMessage()});
            return { std::move(m), lager::noop };
        }

        kzo.client.dbg() << "Uploading identity keys successful" << std::endl;
        m.addTrigger(UploadIdentityKeysSuccessful{});

        m.identityKeysUploaded = true;

        auto &crypto = m.crypto.value();

        crypto.setUploadedOneTimeKeysCount(r.oneTimeKeyCounts());

        return { std::move(m), lager::noop };
    }

}
