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
#include <libkazv-config.hpp>

#include <memory>

#include <maybe.hpp>
#include <event.hpp>

#include "crypto-util.hpp"

namespace Kazv
{
    struct InboundGroupSessionPrivate;

    class InboundGroupSession
    {
    public:
        explicit InboundGroupSession();
        explicit InboundGroupSession(std::string sessionKey, std::string ed25519Key);

        InboundGroupSession(const InboundGroupSession &that);
        InboundGroupSession(InboundGroupSession &&that);
        InboundGroupSession &operator=(const InboundGroupSession &that);
        InboundGroupSession &operator=(InboundGroupSession &&that);
        ~InboundGroupSession();

        MaybeString decrypt(std::string message, std::string eventId, Timestamp originServerTs);

        bool valid() const;

        std::string ed25519Key() const;
    private:
        std::unique_ptr<InboundGroupSessionPrivate> m_d;
    };
}
