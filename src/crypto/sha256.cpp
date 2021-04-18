/*
 * Copyright (C) 2021 Tusooa Zhu <tusooa@kazv.moe>
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

#include <cryptopp/sha.h>

#include "base64.hpp"

#include "sha256.hpp"

namespace Kazv
{
    struct SHA256Desc::Private
    {
        using ImplT = CryptoPP::SHA256;
        ImplT algo;
    };

    SHA256Desc::SHA256Desc()
        : m_d(std::make_unique<Private>())
    {
    }

    KAZV_DEFINE_COPYABLE_UNIQUE_PTR(SHA256Desc, m_d)

    SHA256Desc::~SHA256Desc() = default;

    SHA256Desc SHA256Desc::process(DataT data) const &
    {
        auto next = *this;
        next.processInPlace(std::move(data));
        return next;
    }

    SHA256Desc SHA256Desc::process(DataT data) &&
    {
        auto next = std::move(*this);
        next.processInPlace(std::move(data));
        return next;
    }

    void SHA256Desc::processInPlace(DataT data)
    {
        m_d->algo.Update(reinterpret_cast<const unsigned char *>(data.data()), data.size());
    }

    std::string SHA256Desc::get() const
    {
        auto algo = m_d->algo;
        std::string res(algo.DigestSize(), 0);
        algo.Final(reinterpret_cast<unsigned char *>(res.data()));

        return encodeBase64(res);
    }
}
