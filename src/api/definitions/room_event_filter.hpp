
#pragma once
#include "event_filter.hpp"

namespace Kazv
{
    struct RoomEventFilter : public EventFilter
    {
        bool lazyLoadMembers;
        bool includeRedundantMembers;
        immer::array<std::string> notRooms;
        immer::array<std::string> rooms;
        bool containsUrl;
    };
}
