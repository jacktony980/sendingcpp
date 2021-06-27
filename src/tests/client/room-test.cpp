/*
 * This file is part of libkazv.
 * SPDX-FileCopyrightText: 2021 Tusooa Zhu <tusooa@kazv.moe>
 * SPDX-License-Identifier: AGPL-3.0-or-later
 */

#include <libkazv-config.hpp>

#include <catch2/catch.hpp>

#include <lager/event_loop/boost_asio.hpp>
#include <boost/asio.hpp>

#include <sdk.hpp>
#include <cprjobhandler.hpp>
#include <lagerstoreeventemitter.hpp>
#include <asio-promise-handler.hpp>

using namespace Kazv;

TEST_CASE("Result of Room::toEventLoop() should not vary with the original cursor", "[client][room]")
{
    auto io = boost::asio::io_context{};
    auto jh = Kazv::CprJobHandler{io.get_executor()};
    auto ee = Kazv::LagerStoreEventEmitter(lager::with_boost_asio_event_loop{io.get_executor()});

    auto roomWithId = [](std::string roomId) {
                          auto r = RoomModel{};
                          r.roomId = roomId;
                          return r;
                      };

    auto initModel = Kazv::SdkModel{};
    initModel.client.roomList.rooms = std::move(initModel.client.roomList.rooms)
        .set("!foo:example.org", roomWithId("!foo:example.org"))
        .set("!bar:example.org", roomWithId("!bar:example.org"));
    auto sdk = Kazv::makeSdk(
        initModel,
        jh,
        ee,
        Kazv::AsioPromiseHandler{io.get_executor()},
        zug::identity
        );

    auto ctx = sdk.context();

    auto client = sdk.client();

    auto roomId = lager::make_state(std::string("!foo:example.org"), lager::automatic_tag{});

    auto room = client.roomByCursor(roomId);

    using namespace Kazv::CursorOp;

    REQUIRE(+room.roomId() == "!foo:example.org");

    auto roomInEventLoop = room.toEventLoop();

    REQUIRE(+roomInEventLoop.roomId() == "!foo:example.org");

    roomId.set("!bar:example.org");

    REQUIRE(+room.roomId() == "!bar:example.org");
    REQUIRE(+roomInEventLoop.roomId() == "!foo:example.org");
}
