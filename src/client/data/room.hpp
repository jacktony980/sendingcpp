
#pragma once

#ifndef NDEBUG
#include <lager/debug/cereal/struct.hpp>
#include <lager/debug/cereal/immer_flex_vector.hpp>
#include "cereal_map.hpp"
#endif

#include <string>
#include <variant>
#include <immer/flex_vector.hpp>
#include <immer/map.hpp>

#include "csapi/sync.hpp"
#include "event.hpp"


namespace Kazv
{
    struct Room
    {
        enum Membership
        {
            Invite, Join, Leave
        };

        std::string roomId;
        immer::flex_vector<Event> stateEvents;
        immer::flex_vector<Event> timeline;
        immer::flex_vector<Event> accountData;
        Membership membership;

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

    struct RoomList
    {
        immer::map<std::string, Room> rooms;

        struct UpdateRoomAction
        {
            std::string roomId;
            Room::Action roomAction;
        };

        struct LoadRoomsFromSyncAction
        {
            SyncJob::Rooms rooms;
        };

        using Action = std::variant<
            UpdateRoomAction,
            LoadRoomsFromSyncAction
            >;
        static RoomList update(RoomList l, Action a);
    };
#ifndef NDEBUG
    LAGER_CEREAL_STRUCT(Room::AddStateEventsAction);
    LAGER_CEREAL_STRUCT(Room::AppendTimelineAction);
    LAGER_CEREAL_STRUCT(Room::PrependTimelineAction);
    LAGER_CEREAL_STRUCT(Room::AddAccountDataAction);
    LAGER_CEREAL_STRUCT(Room::ChangeMembershipAction);
    LAGER_CEREAL_STRUCT(Room, (roomId)(stateEvents)(timeline)(accountData)(membership));
    LAGER_CEREAL_STRUCT(RoomList::UpdateRoomAction);
    LAGER_CEREAL_STRUCT(RoomList::LoadRoomsFromSyncAction);
    LAGER_CEREAL_STRUCT(RoomList, (rooms));
#endif
}
