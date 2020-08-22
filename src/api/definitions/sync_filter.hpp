
#pragma once
#include <string>
#include <immer/array.hpp>
#include <descendent.hpp>

#include "types.hpp"
#include "event_filter.hpp"
#include "room_event_filter.hpp"

namespace Kazv
{
    struct RoomFilter;

    struct Filter
    {
        immer::array<std::string> eventFields;
        // eventFormat ignored
        EventFilter presence;
        EventFilter accountData;
        RoomFilter room;

        json toJson() const {
            json j = json::object();
            if (eventFields)
            j["event_fields"] =
        };
    };

    using StateFilter = RoomEventFilter;

    struct RoomFilter
    {
        immer::array<std::string> notRooms;
        immer::array<std::string> rooms;
        RoomEventFilter ephemeral;
        bool includeLeave;
        StateFilter state;
        RoomEventFilter timeline;
        RoomEventFilter accountData;
    };
}
