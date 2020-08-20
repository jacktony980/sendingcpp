
#pragma once
#include <iostream>
#include <boost/iostreams/stream.hpp>

#ifndef LIBKAZV_OUTPUT_LEVEL
#ifndef NDEBUG
#define LIBKAZV_OUTPUT_LEVEL 100
#else
#define LIBKAZV_OUTPUT_LEVEL 0
#endif // NDEBUG
#endif // LIBKAZV_OUTPUT_LEVEL

#define LIBKAZV_OUTPUT_LEVEL_DEBUG 90
#define LIBKAZV_OUTPUT_LEVEL_INFO 70
#define LIBKAZV_OUTPUT_LEVEL_QUIET 20

namespace Kazv
{
    namespace detail
    {
        extern boost::iostreams::stream<boost::iostreams::null_sink> voidOutputHelper;
    }
}


#if LIBKAZV_OUTPUT_LEVEL >= LIBKAZV_OUTPUT_LEVEL_DEBUG
#define dbgApi std::cerr
#define dbgClient std::cerr
#else
#define dbgApi ::Kazv::detail::voidOutputHelper
#define dbgClient ::Kazv::detail::voidOutputHelper
#endif
