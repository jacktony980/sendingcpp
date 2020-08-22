
#pragma once
#include <string>
#include <immer/array.hpp>


namespace Kazv
{
    struct EventFilter
    {
        int limit;
        immer::array<std::string> notSenders;
        immer::array<std::string> notTypes;
        immer::array<std::string> senders;
        immer::array<std::string> types;
    };
}
