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

#include "aes-256-ctr.hpp"

namespace Kazv
{
    struct AES256CTRDesc::Private
    {
        using ImplT = CryptoPP::CTR_Mode<CryptoPP::AES>::Encryption;
        bool valid{true};
        ImplT algo;
    };

    AES256CTRDesc::AES256CTRDesc(SecureData key, std::string iv)
        : m_d(std::make_unique<Private>())
    {
        if (key.size() != keySize || iv.size() != ivSize) {
            m_d->valid = false;
        } else {
            m_d->valid = true;
            m_d->algo.SetKeyWithIV(reinterpret_cast<const unsigned char *>(key.data()),
                                   key.size(),
                                   reinterpret_cast<const unsigned char *>(iv.data()));
        }
    }

    KAZV_DEFINE_COPYABLE_UNIQUE_PTR(AES256CTRDesc, m_d)

    AES256CTRDesc::~AES256CTRDesc() = default;

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
