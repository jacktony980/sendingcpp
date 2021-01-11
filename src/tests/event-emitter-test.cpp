/*
 * Copyright (C) 2020 Tusooa Zhu <tusooa@vista.aero>
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

#include <chrono>

#include <catch2/catch.hpp>

#include <eventemitter/lagerstoreeventemitter.hpp>

#include <lager/event_loop/boost_asio.hpp>


using namespace Kazv;

TEST_CASE("Event emitter should work normally", "[eventemitter]")
{
    boost::asio::io_context ioContext;

    LagerStoreEventEmitter ee{lager::with_boost_asio_event_loop{ioContext.get_executor()}};

    auto watchable = ee.watchable();
    int counter = 0;

    watchable.after<SyncSuccessful>(
        [&](auto) {
            ++counter;
        });

    SECTION("Event handlers should be run properly") {
        ee.emit(SyncSuccessful{});
        ee.emit(SyncFailed{});

        ioContext.run();

        REQUIRE(counter == 1);
    }

    SECTION("Event handlers should be called when every event fires off") {
        for (int i = 0; i < 100; ++i) {
            ee.emit(SyncSuccessful{});
        }

        ioContext.run();

        REQUIRE(counter == 100);
    }

    SECTION("Handler should be disconnected once watchable is destroyed") {
        int counter2 = 0;

        auto guard = boost::asio::executor_work_guard(ioContext.get_executor());
        auto thread = std::thread([&] { ioContext.run(); });

        ee.emit(SyncFailed{});
        std::this_thread::sleep_for(std::chrono::milliseconds{100});

        {
            auto watchable2 = ee.watchable();
            watchable2.after<SyncFailed>(
                [&](auto) {
                    ++counter2;
                });

            ee.emit(SyncSuccessful{});
            ee.emit(SyncFailed{});
            std::this_thread::sleep_for(std::chrono::milliseconds{100});
        }

        ee.emit(SyncFailed{});

        guard.reset();

        thread.join();

        REQUIRE(counter == 1);
        REQUIRE(counter2 == 1);
    }
}
