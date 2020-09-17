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


#include <regex>
#include <tuple>
#include <cereal/archives/json.hpp>
#include <lager/lenses/optional.hpp>

#include <zug/into.hpp>
#include <zug/compose.hpp>
#include <immer/flex_vector_transient.hpp>
#include <immer/flex_vector.hpp>

#include <client/clientwrap.hpp>
#include "commands.hpp"

using namespace std::string_literals;

static std::regex roomMsgsRegex("room msgs (.+)");
static std::regex roomStatesRegex("room states (.+)");
static std::regex roomMemsRegex("room mems (.+)");
static std::regex roomSendRegex("room send ([^\\s]+) (.+)");


void parse(std::string l, Kazv::ClientWrap c)
{
    using namespace Kazv::CursorOp;
    std::smatch m;
    if (l == "rooms") {
        auto roomIds = +c.roomIds();

        std::cout << "Room Id\tRoom Name\n";
        for (auto id : roomIds) {
            auto roomName = c.room(id).name().make().get();
            std::cout << id << "\t" << roomName << "\n";
        }

    } else if (std::regex_match(l, m, roomMsgsRegex)) {
        auto roomId = m[1].str();
        auto room = c.room(roomId);

        auto msgs = +room.timelineEvents();

        std::cout << "Messages in " << roomId << ":\n";
        for (auto msg : msgs) {
            std::cout << "Event "
                      << msg.id() << " from "
                      << msg.sender() << " at "
                      << msg.originServerTs() << " typed "
                      << msg.type() << "\n";
            {
                cereal::JSONOutputArchive archive( std::cout );
                archive(msg.content());
            }
            std::cout << std::endl;
        }

    } else if (std::regex_match(l, m, roomMemsRegex)) {
        auto roomId = m[1].str();
        auto room = c.room(roomId);
        auto members = +room.members();

        std::cout << "Members in " << roomId << ":\n";
        for (auto userId : members) {
            std::cout << userId << std::endl;
        }

    } else if (std::regex_match(l, m, roomStatesRegex)) {
        auto roomId = m[1].str();
        auto room = c.room(roomId);
        auto states = +room.stateEvents();

        std::cout << "States in " << roomId << ":\n";
        for (auto [k, st] : states) {
            auto [type, stateKey] = k;
            std::cout << "State typed "
                      << type << " with stateKey "
                      << stateKey << "\n";
            {
                cereal::JSONOutputArchive archive( std::cout );
                archive(st);
            }
            std::cout << std::endl;
        }
    } else if (std::regex_match(l, m, roomSendRegex)) {
        auto roomId = m[1].str();
        auto text = m[2].str();
        auto room = c.room(roomId);

        room.sendTextMessage(text);
    } else {
        // no valid action, display help
        std::cout << "Commands:\n"
                  << "rooms -- List rooms\n"
                  << "room msgs <roomId> -- List room messages\n"
                  << "room states <roomId> -- List room states\n"
                  << "room mems <roomId> -- List room members\n"
                  << "room send <roomId> <message text> -- Send a text message\n\n";

    }
}
