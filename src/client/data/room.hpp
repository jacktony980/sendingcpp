
#pragma once
#include <string>
#include <variant>
#include <immer/flex_vector.hpp>
#include <immer/map.hpp>

#include "event.hpp"

namespace Kazv
{
    struct Room
    {
        std::string roomId;
        immer::flex_vector<Event> stateEvents;
        immer::flex_vector<Event> timeline;

        enum Membership
        {
            Invite, Join, Leave
        };

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

        using Action = std::variant<
            AddStateEventsAction,
            AppendTimelineAction,
            PrependTimelineAction
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

        using Action = std::variant<UpdateRoomAction>;
        static RoomList update(RoomList l, Action a);
    };
}
