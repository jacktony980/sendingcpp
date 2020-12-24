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

#include <vector>

#include "debug.hpp"

namespace Kazv
{
    namespace detail
    {
        boost::iostreams::stream<boost::iostreams::null_sink> voidOutputHelper{boost::iostreams::null_sink()};

        std::ostream &OutputHelper::basicFormat() const
        {
            auto now = std::chrono::system_clock::now();
            std::time_t nowTimeT = std::chrono::system_clock::to_time_t(now);

            std::vector<char> timeStr(40, '0');
            std::strftime(timeStr.data(), 40, "%Y,%-m,%-d (%u) %H,%M,%S", std::localtime(&nowTimeT));
            return std::cerr << "[" << timeStr.data() << "]["
                             << category << "]["
                             << severity << "] ";
        }
    }
}

#if LIBKAZV_OUTPUT_LEVEL >= LIBKAZV_OUTPUT_LEVEL_DEBUG
const ::Kazv::detail::OutputHelper dbgApi{"api", "debug"};
const ::Kazv::detail::OutputHelper dbgClient{"client", "debug"};
const ::Kazv::detail::OutputHelper dbgJob{"job", "debug"};
#endif
