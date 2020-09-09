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

#include <lager/lenses/optional.hpp>

#include "clientwrap.hpp"

namespace Kazv
{
    struct ClientWrap::Private
    {
        lager::reader<Client> client;
        lager::context<Client::Action> ctx;
    };

    ClientWrap::ClientWrap(lager::reader<Client> client,
                           lager::context<Client::Action> ctx)
        : m_d{client, ctx}
    {}

    ClientWrap::~ClientWrap() = default;

    lager::reader<immer::map<std::string, Room>> ClientWrap::rooms() const
    {
        return m_d->client
            [&Client::roomList]
            [&RoomList::rooms];
    }

    lager::reader<immer::flex_vector<std::string>> ClientWrap::roomIds() const
    {
        return rooms().xform(
            zug::map([](auto m) {
                         return zug::into(
                             immer::flex_vector_transient{},
                             zug::map([](auto val) { return val.first; }),
                             m)
                             .persistent();
                     }));
    }

    lager::reader<RoomWrap> roomAt(std::string id) const
    {
        return rooms()[id];
    }

    lager::reader<Room> roomAtOrDefault(std::string id) const {
        return roomAt(std::move(id))[lager::lenses::or_default];
    }

}
