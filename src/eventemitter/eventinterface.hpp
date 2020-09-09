/*
 * Copyright (C) 2020 Tusooa Zhu
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
#include <variant>
#include "types.hpp"
#include "event.hpp"

namespace Kazv
{
    struct LoginSuccessful {};
    inline bool operator==(LoginSuccessful, LoginSuccessful)
    {
        return true;
    }

    struct ReceivingPresenceEvent { Event event; };
    inline bool operator==(ReceivingPresenceEvent a, ReceivingPresenceEvent b)
    {
        return a.event == b.event;
    }

    struct ReceivingAccountDataEvent { Event event; };
    inline bool operator==(ReceivingAccountDataEvent a, ReceivingAccountDataEvent b)
    {
        return a.event == b.event;
    }

    struct ReceivingRoomTimelineEvent {
        Event event;
        std::string roomId;
    };

    inline bool operator==(ReceivingRoomTimelineEvent a, ReceivingRoomTimelineEvent b)
    {
        return a.event == b.event && a.roomId == b.roomId;
    }


    using KazvEvent = std::variant<
        // use this for placeholder of "no events yet"
        // otherwise the first LoginSuccessful event cannot be detected
        std::monostate,
        LoginSuccessful,
        ReceivingPresenceEvent,
        ReceivingAccountDataEvent,
        ReceivingRoomTimelineEvent
        >;

    class EventInterface
    {
    public:
        virtual ~EventInterface() = default;
        virtual void emit(KazvEvent e) = 0;
    };
}
