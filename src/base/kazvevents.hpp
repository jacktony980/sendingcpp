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

    struct ReceivingRoomStateEvent {
        Event event;
        std::string roomId;
    };

    inline bool operator==(ReceivingRoomStateEvent a, ReceivingRoomStateEvent b)
    {
        return a.event == b.event && a.roomId == b.roomId;
    }

    struct ReceivingRoomTimelineEvent {
        Event event;
        std::string roomId;
    };

    inline bool operator==(ReceivingRoomTimelineEvent a, ReceivingRoomTimelineEvent b)
    {
        return a.event == b.event && a.roomId == b.roomId;
    }

    struct ReceivingRoomAccountDataEvent {
        Event event;
        std::string roomId;
    };

    inline bool operator==(ReceivingRoomAccountDataEvent a, ReceivingRoomAccountDataEvent b)
    {
        return a.event == b.event && a.roomId == b.roomId;
    }

    struct RoomMembershipChanged {
        RoomMembership membership;
        std::string roomId;
    };

    inline bool operator==(RoomMembershipChanged a, RoomMembershipChanged b)
    {
        return a.membership == b.membership && a.roomId == b.roomId;
    }

    struct FailedToJoinRoom {
        std::string roomIdOrAlias;
        std::string errorCode;
        std::string error;
    };

    inline bool operator==(FailedToJoinRoom a, FailedToJoinRoom b)
    {
        return a.roomIdOrAlias == b.roomIdOrAlias
            && a.errorCode == b.errorCode
            && a.error == b.error;
    }


    using KazvEvent = std::variant<
        // use this for placeholder of "no events yet"
        // otherwise the first LoginSuccessful event cannot be detected
        std::monostate,
        LoginSuccessful,
        ReceivingPresenceEvent,
        ReceivingAccountDataEvent,
        ReceivingRoomTimelineEvent,
        ReceivingRoomStateEvent,
        RoomMembershipChanged,
        ReceivingRoomAccountDataEvent,

        FailedToJoinRoom
        >;

    using KazvEventList = immer::flex_vector<KazvEvent>;
}
