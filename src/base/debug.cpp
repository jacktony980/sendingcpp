/*
 * Copyright (C) 2020-2021 Tusooa Zhu <tusooa@vista.aero>
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
#include <cstdlib>

#include <boost/algorithm/string.hpp>

#include "debug.hpp"

#ifndef NDEBUG
#define KAZV_DEFAULT_OUTPUT_LEVEL DEBUG
#else
#define KAZV_DEFAULT_OUTPUT_LEVEL WARNING
#endif

namespace Kazv
{
    namespace detail
    {
        boost::iostreams::stream<boost::iostreams::null_sink> voidOutputHelper{boost::iostreams::null_sink()};

        std::string toStr(OutputLevel l)
        {
            if (l == ERROR) {
                return "error";
            } else if (l == WARNING) {
                return "warning";
            } else if (l == INFO) {
                return "info";
            } else if (l == DEBUG) {
                return "debug";
            } else {
                return "none";
            }
        }

        std::ostream &OutputHelper::basicFormat() const
        {
            auto now = std::chrono::system_clock::now();
            std::time_t nowTimeT = std::chrono::system_clock::to_time_t(now);

            std::vector<char> timeStr(40, '0');
            std::strftime(timeStr.data(), 40, "%Y,%-m,%-d (%u) %H,%M,%S", std::localtime(&nowTimeT));
            return std::cerr << "[" << timeStr.data() << "]["
                             << category << "]["
                             << toStr(severity) << "] ";
        }

        OutputHelper OutputGroup::dbg() const { return OutputHelper{name, DEBUG, level}; }
        OutputHelper OutputGroup::info() const { return OutputHelper{name, INFO, level}; }
        OutputHelper OutputGroup::warn() const { return OutputHelper{name, WARNING, level}; }
        OutputHelper OutputGroup::err() const { return OutputHelper{name, ERROR, level}; }

        OutputConfig::OutputConfig()
            : api{"api", KAZV_DEFAULT_OUTPUT_LEVEL}
            , base{"base", KAZV_DEFAULT_OUTPUT_LEVEL}
            , client{"client", KAZV_DEFAULT_OUTPUT_LEVEL}
            , ee{"eventemitter", KAZV_DEFAULT_OUTPUT_LEVEL}
            , job{"job", KAZV_DEFAULT_OUTPUT_LEVEL}
            , crypto{"crypto", KAZV_DEFAULT_OUTPUT_LEVEL}
        {
            auto env = std::getenv("KAZV_OUTPUT_LEVELS");
            std::string s{env ? env : ""};
            using namespace boost::algorithm;

            std::vector<std::string> groups;
            split(groups, s, is_any_of(","));

            for (auto g : groups) {
                std::vector<std::string> cfg;
                split(cfg, g, is_any_of("="));
                if (cfg.size() != 2) {
                    return;
                }
                OutputLevel l;
                if (cfg[1] == "debug") {
                    l = DEBUG;
                } else if (cfg[1] == "info") {
                    l = INFO;
                } else if (cfg[1] == "warning") {
                    l = WARNING;
                } else if (cfg[1] == "error") {
                    l = ERROR;
                } else {
                    l = NONE;
                }

                if (cfg[0] == "api") {
                    api.level = l;
                } else if (cfg[0] == "base") {
                    base.level = l;
                } else if (cfg[0] == "client") {
                    client.level = l;
                } else if (cfg[0] == "eventemitter") {
                    ee.level = l;
                } else if (cfg[0] == "job") {
                    job.level = l;
                } else if (cfg[0] == "crypto") {
                    crypto.level = l;
                }
            }
        }
    }

    const detail::OutputConfig kzo{};
}
