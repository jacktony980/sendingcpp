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
#include <iostream>
#include <chrono>
#include <ctime>

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

        struct OutputHelper
        {
            std::string category;
            std::string severity;

            std::ostream &basicFormat() const;

            template<class T>
            std::ostream &operator<<(T &&arg) const {
                return basicFormat() << std::forward<T>(arg);
            }
        };
    }
}


#if LIBKAZV_OUTPUT_LEVEL >= LIBKAZV_OUTPUT_LEVEL_DEBUG
extern const ::Kazv::detail::OutputHelper dbgApi;
extern const ::Kazv::detail::OutputHelper dbgClient;
extern const ::Kazv::detail::OutputHelper dbgJob;
#else
#define dbgApi ::Kazv::detail::voidOutputHelper
#define dbgClient ::Kazv::detail::voidOutputHelper
#define dbgJob ::Kazv::detail::voidOutputHelper
#endif
