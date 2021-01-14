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

#include "crypto.hpp"

#include "crypto-util.hpp"

namespace Kazv
{
    struct CryptoPrivate
    {
        CryptoPrivate();
        CryptoPrivate(const CryptoPrivate &that);
        ~CryptoPrivate();

        ByteArray accountData;
        OlmAccount *account;

        void checkError(std::size_t code);
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
    {
        auto pickleData = ByteArray(olm_pickle_account_length(account), '\0');
        auto key = ByteArray(3, 'x');
        checkError(olm_pickle_account(that.account, key.data(), key.size(),
                                      pickleData.data(), pickleData.size()));
        checkError(olm_unpickle_account(account, key.data(), key.size(),
                                        pickleData.data(), pickleData.size()));
    }

    void CryptoPrivate::checkError(std::size_t code)
    {
        // TODO
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
}
