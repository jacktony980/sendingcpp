
#pragma once

#include <lager/debug/cereal/struct.hpp>
#include <lager/debug/cereal/immer_flex_vector.hpp>
#include "cereal_map.hpp"

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
        immer::flex_vector<Event> timeline;
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
