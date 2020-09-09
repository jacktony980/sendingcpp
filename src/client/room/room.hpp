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

#include <lager/debug/cereal/struct.hpp>
#include <lager/debug/cereal/immer_flex_vector.hpp>
#include "client/data/cereal_map.hpp"

#include <string>
#include <variant>
#include <immer/flex_vector.hpp>
#include <immer/map.hpp>

#include "csapi/sync.hpp"
#include "event.hpp"
#include "client/util.hpp"

namespace Kazv
{
    struct Room
    {
        enum Membership
        {
            Invite, Join, Leave
        };

        std::string roomId;
        immer::map<KeyOfState, Event> stateEvents;
        immer::flex_vector<std::string> timeline;
        immer::map<std::string, Event> messages;
        immer::map<std::string, Event> accountData;
        Membership membership;
        std::string paginateBackToken;
        /// whether this room has earlier events to be fetched
        bool canPaginateBack{true};

        struct AddStateEventsAction
        {
            immer::flex_vector<Event> stateEvents;
        };

        struct AppendTimelineAction
        {
            immer::flex_vector<Event> events;
        };

        struct PrependTimelineAction
        {
            immer::flex_vector<Event> events;
            std::string paginateBackToken;
        };

        struct AddAccountDataAction
        {
            immer::flex_vector<Event> events;
        };

        struct ChangeMembershipAction
        {
            Membership membership;
        };

        using Action = std::variant<
            AddStateEventsAction,
            AppendTimelineAction,
            PrependTimelineAction,
            AddAccountDataAction,
            ChangeMembershipAction
            >;

        static Room update(Room r, Action a);
    };

    inline bool operator==(Room a, Room b)
    {
        return a.roomId == b.roomId
            && a.stateEvents == b.stateEvents
            && a.timeline == b.timeline
            && a.messages == b.messages
            && a.accountData == b.accountData
            && a.membership == b.membership
            && a.paginateBackToken == b.paginateBackToken
            && a.canPaginateBack == b.canPaginateBack;
    }

    struct RoomList
    {
        immer::map<std::string, Room> rooms;

        inline auto at(std::string id) const { return rooms.at(id); }

        struct UpdateRoomAction
        {
            std::string roomId;
            Room::Action roomAction;
        };

        using Action = std::variant<
            UpdateRoomAction
            >;
        static RoomList update(RoomList l, Action a);
    };

    inline bool operator==(RoomList a, RoomList b)
    {
        return a.rooms == b.rooms;
    }

#ifndef NDEBUG
    LAGER_CEREAL_STRUCT(Room::AddStateEventsAction);
    LAGER_CEREAL_STRUCT(Room::AppendTimelineAction);
    LAGER_CEREAL_STRUCT(Room::PrependTimelineAction);
    LAGER_CEREAL_STRUCT(Room::AddAccountDataAction);
    LAGER_CEREAL_STRUCT(Room::ChangeMembershipAction);
    LAGER_CEREAL_STRUCT(RoomList::UpdateRoomAction);
#endif

    template<class Archive>
    void serialize(Archive &ar, Room &r, std::uint32_t const /*version*/)
    {
        ar(r.roomId,
           r.stateEvents,
           r.timeline,
           r.messages,
           r.accountData,
           r.membership,
           r.paginateBackToken,
           r.canPaginateBack);
    }

    template<class Archive>
    void serialize(Archive &ar, RoomList &l, std::uint32_t const /*version*/)
    {
        ar(l.rooms);
    }
}

CEREAL_CLASS_VERSION(Kazv::Room, 0);
CEREAL_CLASS_VERSION(Kazv::RoomList, 0);
