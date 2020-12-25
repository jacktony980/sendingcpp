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

namespace Kazv
{
    namespace detail
    {
        extern boost::iostreams::stream<boost::iostreams::null_sink> voidOutputHelper;

        enum OutputLevel
        {
            NONE,
            ERROR,
            WARNING,
            INFO,
            DEBUG,
        };

        struct OutputHelper
        {
            std::string category;
            OutputLevel severity;
            OutputLevel level;

            std::ostream &basicFormat() const;

            template<class T>
            std::ostream &operator<<(T &&arg) const {
                if (severity <= level) {
                    return basicFormat() << std::forward<T>(arg);
                } else {
                    return voidOutputHelper;
                }
            }
        };

        struct OutputGroup
        {
            std::string name;
            OutputLevel level;

            OutputHelper dbg() const;
            OutputHelper info() const;
            OutputHelper warn() const;
            OutputHelper err() const;
        };

        struct OutputConfig
        {
            OutputConfig();
            OutputGroup api;
            OutputGroup base;
            OutputGroup client;
            OutputGroup ee;
            OutputGroup job;
        };
    }

    extern const detail::OutputConfig kzo;
}
