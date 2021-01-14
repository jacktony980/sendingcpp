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

#pragma once

#include <memory>

#include "crypto-util.hpp"

namespace Kazv
{
    struct CryptoPrivate;
    class Crypto
    {
    public:
        explicit Crypto();
        Crypto(const Crypto &that);
        Crypto(Crypto &&that);
        Crypto &operator=(const Crypto &that);
        Crypto &operator=(Crypto &&that);
        ~Crypto();

        ByteArray identityKeys();

    private:
        std::unique_ptr<CryptoPrivate> m_d;
    };
}
