/*
 * This file is part of libkazv.
 * SPDX-FileCopyrightText: 2021 Tusooa Zhu <tusooa@kazv.moe>
 * SPDX-License-Identifier: AGPL-3.0-or-later
 */

#ifndef NDEBUG
#include <libkazv-config.hpp>

#include <catch2/catch.hpp>

#include <lager/event_loop/boost_asio.hpp>
#include <boost/asio.hpp>

#include <sdk.hpp>
#include <cprjobhandler.hpp>
#include <lagerstoreeventemitter.hpp>
#include <asio-promise-handler.hpp>

using namespace Kazv;

TEST_CASE("Thread-safety verification should work", "[client][thread-safety]")
{
    auto io = boost::asio::io_context{};
    auto jh = Kazv::CprJobHandler{io.get_executor()};
    auto ee = Kazv::LagerStoreEventEmitter(lager::with_boost_asio_event_loop{io.get_executor()});

    auto sdk = Kazv::makeSdk(
        Kazv::SdkModel{},
        jh,
        ee,
        Kazv::AsioPromiseHandler{io.get_executor()},
        zug::identity
        );

    auto ctx = sdk.context();

    REQUIRE(ctx.has<EventLoopThreadIdKeeper>());

    auto client = sdk.client();

    bool thrown = false;
    try {
        client.userId();
    } catch (const ThreadNotMatchException &) {
        thrown = true;
    }
    REQUIRE(! thrown); // event loop has not started yet, so do not throw

    boost::asio::executor_work_guard g(io.get_executor());

    std::thread([&io] { io.run(); }).detach();

    // wait till the event loop thread is logged
    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    thrown = false;
    std::thread([=, &thrown] {
                    try {
                        client.userId();
                    } catch (const ThreadNotMatchException &) {
                        thrown = true;
                    }
                }).join();
    REQUIRE(thrown);

    ctx.createResolvedPromise({})
        .then([](auto &&) {})
        .then([&client](auto &&) {
                  client.userId();
              });

    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    thrown = false;
    try {
        client.userId();
    } catch (const ThreadNotMatchException &) {
        thrown = true;
    }
    REQUIRE(thrown);
}


#endif
