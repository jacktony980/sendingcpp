/*
 * Copyright (C) 2020 Tusooa Zhu <tusooa@vista.aero>
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

#include <vector>

#include <olm/olm.h>

#include <nlohmann/json.hpp>

#include <debug.hpp>

#include "crypto.hpp"

#include "crypto-util.hpp"

namespace Kazv
{
    using namespace CryptoConstants;

    struct CryptoPrivate
    {
        CryptoPrivate();
        CryptoPrivate(const CryptoPrivate &that);
        ~CryptoPrivate();

        ByteArray accountData;
        OlmAccount *account;
        immer::map<std::string /* algorithm */, int> uploadedOneTimeKeysCount;
        int numUnpublishedKeys{0};

        std::size_t checkError(std::size_t code) const;
    };

    CryptoPrivate::CryptoPrivate()
        : accountData(olm_account_size(), 0)
        , account(olm_account(accountData.data()))
    {
        auto randLen = olm_create_account_random_length(account);
        auto randomData = genRandom(randLen);
        checkError(olm_create_account(account, randomData.data(), randLen));
    }

    CryptoPrivate::~CryptoPrivate()
    {
        olm_clear_account(account);
    }

    CryptoPrivate::CryptoPrivate(const CryptoPrivate &that)
        : accountData(olm_account_size(), 0)
        , account(olm_account(accountData.data()))
        , uploadedOneTimeKeysCount(that.uploadedOneTimeKeysCount)
        , numUnpublishedKeys(that.numUnpublishedKeys)
    {
        auto pickleData = ByteArray(olm_pickle_account_length(that.account), '\0');
        auto key = ByteArray(3, 'x');
        // XXX: olm_pickle_account is technically taking a (non-const) OlmAccount *
        // but error will be set only if there is some problem...
        that.checkError(olm_pickle_account(that.account, key.data(), key.size(),
                                           pickleData.data(), pickleData.size()));
        checkError(olm_unpickle_account(account, key.data(), key.size(),
                                        pickleData.data(), pickleData.size()));
    }

    std::size_t CryptoPrivate::checkError(std::size_t code) const
    {
        if (code == olm_error()) {
            kzo.crypto.warn() << "Olm error: " << olm_account_last_error(account) << std::endl;
        }
        return code;
    }


    Crypto::Crypto()
        : m_d(new CryptoPrivate{})
    {
    }

    Crypto::~Crypto() = default;

    Crypto::Crypto(const Crypto &that)
        : m_d(new CryptoPrivate(*that.m_d))
    {
    }

    Crypto::Crypto(Crypto &&that)
        : m_d(std::move(that.m_d))
    {
    }

    Crypto &Crypto::operator=(const Crypto &that)
    {
        m_d.reset(new CryptoPrivate(*that.m_d));
        return *this;
    }

    Crypto &Crypto::operator=(Crypto &&that)
    {
        m_d = std::move(that.m_d);
        return *this;
    }

    ByteArray Crypto::identityKeys()
    {
        auto ret = ByteArray(olm_account_identity_keys_length(m_d->account), '\0');
        m_d->checkError(olm_account_identity_keys(m_d->account, ret.data(), ret.size()));
        return ret;
    }

    std::string Crypto::ed25519IdentityKey()
    {
        auto keys = identityKeys();
        auto keyStr = std::string(keys.begin(), keys.end());
        auto keyJson = nlohmann::json::parse(keyStr);
        return keyJson.at(ed25519);
    }

    std::string Crypto::curve25519IdentityKey()
    {
        auto keys = identityKeys();
        auto keyStr = std::string(keys.begin(), keys.end());
        auto keyJson = nlohmann::json::parse(keyStr);
        return keyJson.at(curve25519);
    }

    std::string Crypto::sign(nlohmann::json j)
    {
        j.erase("signatures");
        j.erase("unsigned");

        auto str = j.dump();

        auto ret = ByteArray(olm_account_signature_length(m_d->account), '\0');

        kzo.crypto.dbg() << "We are about to sign: " << str << std::endl;

        m_d->checkError(olm_account_sign(m_d->account,
                                         str.data(), str.size(),
                                         ret.data(), ret.size()));

        return std::string{ret.begin(), ret.end()};
    }

    void Crypto::setUploadedOneTimeKeysCount(immer::map<std::string /* algorithm */, int> uploadedOneTimeKeysCount)
    {
        m_d->uploadedOneTimeKeysCount = uploadedOneTimeKeysCount;
    }

    int Crypto::maxNumberOfOneTimeKeys()
    {
        return olm_account_max_number_of_one_time_keys(m_d->account);
    }

    void Crypto::genOneTimeKeys(int num)
    {
        auto random = genRandom(olm_account_generate_one_time_keys_random_length(m_d->account, num));
        auto res = m_d->checkError(
            olm_account_generate_one_time_keys(
                m_d->account,
                num,
                random.data(), random.size()));

        if (res != olm_error()) {
            m_d->numUnpublishedKeys += num;
        }
    }

    nlohmann::json Crypto::unpublishedOneTimeKeys()
    {
        auto keys = ByteArray(olm_account_one_time_keys_length(m_d->account), '\0');
        m_d->checkError(olm_account_one_time_keys(m_d->account, keys.data(), keys.size()));

        return nlohmann::json::parse(std::string(keys.begin(), keys.end()));
    }

    void Crypto::markOneTimeKeysAsPublished()
    {
        auto ret = m_d->checkError(olm_account_mark_keys_as_published(m_d->account));
        if (ret != olm_error()) {
            m_d->numUnpublishedKeys = 0;
        }
    }

    int Crypto::numUnpublishedOneTimeKeys() const
    {
        return m_d->numUnpublishedKeys;
    }

    int Crypto::uploadedOneTimeKeysCount(std::string algorithm) const
    {
        return m_d->uploadedOneTimeKeysCount[algorithm];
    }
}
