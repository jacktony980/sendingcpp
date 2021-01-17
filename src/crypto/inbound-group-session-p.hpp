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

#include "inbound-group-session.hpp"

#include <olm/olm.h>

#include <immer/map.hpp>

namespace Kazv
{
    struct KeyOfDecryptedEvent
    {
        std::string eventId;
        Timestamp originServerTs;
    };

    inline bool operator==(KeyOfDecryptedEvent a, KeyOfDecryptedEvent b)
    {
        return a.eventId == b.eventId
            && a.originServerTs == b.originServerTs;
    }

    inline bool operator!=(KeyOfDecryptedEvent a, KeyOfDecryptedEvent b)
    {
        return !(a == b);
    }

    struct InboundGroupSessionPrivate
    {
        InboundGroupSessionPrivate();
        InboundGroupSessionPrivate(std::string sessionKey, std::string ed25519Key);
        InboundGroupSessionPrivate(const InboundGroupSessionPrivate &that);
        ~InboundGroupSessionPrivate() = default;

        ByteArray sessionData;
        OlmInboundGroupSession *session;

        std::string ed25519Key;

        bool valid{false};

        immer::map<std::uint32_t /* index */, KeyOfDecryptedEvent> decryptedEvents;

        std::size_t checkError(std::size_t code) const;
        std::string error() const;

        ByteArray pickle() const;
        bool unpickle(ByteArray pickleData);
    };

}
