/*
 * Copyright (C) 2021 Tusooa Zhu <tusooa@kazv.moe>
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

#include <libkazv-config.hpp>

#include <catch2/catch.hpp>

#include <event.hpp>

using namespace Kazv;


TEST_CASE("Should distinguish between state events and other events", "[base][event]")
{
    Event state = R"({
    "state_key": "",
    "sender": "@example:example.org",
    "type": "moe.kazv.mxc.custom.state.type",
    "event_id": "!dummy",
    "origin_server_ts": 1234,
    "content": {}
})"_json;

    REQUIRE(state.isState());

    Event nonState = R"({
    "sender": "@example:example.org",
    "type": "moe.kazv.mxc.custom.non.state.type",
    "event_id": "!dummy",
    "origin_server_ts": 1234,
    "content": {}
})"_json;

    REQUIRE(!nonState.isState());
}
