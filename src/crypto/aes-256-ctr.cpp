/*
 * Copyright (C) 2020-2021 Tusooa Zhu <tusooa@kazv.moe>
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

#include <libkazv-config.hpp>


#include <cryptopp/aes.h>
#include <cryptopp/ccm.h>

#include "base64.hpp"

#include "aes-256-ctr.hpp"

namespace Kazv
{
    struct AES256CTRDesc::Private
    {
        using ImplT = CryptoPP::CTR_Mode<CryptoPP::AES>::Encryption;
        bool valid{true};
        std::string key;
        std::string iv;
        ImplT algo;
    };

    AES256CTRDesc::AES256CTRDesc(RawTag, DataT key, DataT iv)
        : m_d(std::make_unique<Private>())
    {
        if (key.size() != keySize || iv.size() != ivSize) {
            m_d->valid = false;
        } else {
            m_d->valid = true;
            m_d->key = encodeBase64(key, Base64Opts::urlSafe);
            m_d->iv = encodeBase64(iv);
            m_d->algo.SetKeyWithIV(reinterpret_cast<const unsigned char *>(key.data()),
                                   key.size(),
                                   reinterpret_cast<const unsigned char *>(iv.data()));
        }
    }


    AES256CTRDesc::AES256CTRDesc(std::string key, std::string iv)
        : AES256CTRDesc(RawTag{}, decodeBase64(key, Base64Opts::urlSafe), decodeBase64(iv))
    {
    }

    KAZV_DEFINE_COPYABLE_UNIQUE_PTR(AES256CTRDesc, m_d)

    AES256CTRDesc::~AES256CTRDesc() = default;

    bool AES256CTRDesc::valid() const
    {
        return m_d && m_d->valid;
    }

    std::string AES256CTRDesc::key() const
    {
        return m_d->key;
    }

    std::string AES256CTRDesc::iv() const
    {
        return m_d->iv;
    }

    auto AES256CTRDesc::process(DataT data) const & -> Result
    {
        auto next = *this;
        auto res = next.processInPlace(data);
        return { std::move(next), std::move(res) };
    }

    auto AES256CTRDesc::process(DataT data) && -> Result
    {
        auto next = std::move(*this);
        auto res = next.processInPlace(data);
        return { std::move(next), std::move(res) };
    }

    auto AES256CTRDesc::processInPlace(DataT data) -> DataT
    {
        m_d->algo.ProcessString(reinterpret_cast<unsigned char *>(data.data()), data.size());
        return data;
    }
}
