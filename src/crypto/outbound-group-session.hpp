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

#pragma once

#include <memory>

#include <maybe.hpp>
#include <event.hpp>

#include "crypto-util.hpp"

namespace Kazv
{
    struct OutboundGroupSessionPrivate;

    class OutboundGroupSession
    {
    public:
        explicit OutboundGroupSession();

        OutboundGroupSession(const OutboundGroupSession &that);
        OutboundGroupSession(OutboundGroupSession &&that);
        OutboundGroupSession &operator=(const OutboundGroupSession &that);
        OutboundGroupSession &operator=(OutboundGroupSession &&that);
        ~OutboundGroupSession();

        std::string encrypt(std::string plainText);

        bool valid() const;

        std::string sessionKey();
        std::string sessionId();

        int messageIndex();

        Timestamp creationTimeMs() const;
    private:
        std::unique_ptr<OutboundGroupSessionPrivate> m_d;
    };
}
