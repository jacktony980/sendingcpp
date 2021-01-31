/*
 * Copyright (C) 2021 Tusooa Zhu <tusooa@vista.aero>
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

#include <catch2/catch.hpp>

#include <asio-promise-handler.hpp>

using namespace Kazv;

struct MockDataStruct
{
    int i{};
};

TEST_CASE("Promise should behave properly", "[promise]")
{
    boost::asio::io_context ioContext;
    auto ph = AsioPromiseHandler(ioContext.get_executor());
    std::vector<int> v;

    auto p1 = ph.create<int>([&v](auto resolve) {
                                 v.push_back(1);
                                 resolve(2);
                             });

    auto p2 = p1.then([&v, &ph](int val) {
                          v.push_back(val);
                          return ph.createResolved(3);
                      });

    auto p3 = p2.then([&v, &ph](int val) {
                          v.push_back(val);
                          return ph.createResolved(-1);
                      });

    auto p4 = p3.then([](int val) {
                          REQUIRE(val == -1);
                          return 5;
                      });

    auto p5 = p4.then([](int val) {
                          REQUIRE(val == 5);
                          return MockDataStruct{6};
                      });

    auto p6 = p5.then([](MockDataStruct m) {
                          REQUIRE(m.i == 6);
                          return 0;
                      });

    auto pTimer = ph.create<int>([&ioContext](auto resolve) {
                                     auto timer = std::make_shared<boost::asio::steady_timer>(ioContext);
                                     timer->expires_after(std::chrono::milliseconds(300));
                                     timer->async_wait(
                                         [timer, resolve](const boost::system::error_code& error) {
                                             if (! error) {
                                                 resolve(20);
                                             }
                                         });
                                 });

    auto pTimer2 = pTimer.then([](int val) {
                                   REQUIRE(val == 20);
                                   return 0;
                               });

    ioContext.run();

    REQUIRE(v == std::vector<int>{ 1, 2, 3 });
}


TEST_CASE("BoolPromise should behave properly", "[promise]")
{
    boost::asio::io_context ioContext;
    auto ph = BoolPromiseInterface(AsioPromiseHandler(ioContext.get_executor()));

    auto p1 = ph.create([](auto resolve) {
                            resolve(true);
                        });
    auto p2 = p1.then([](bool v) {
                          REQUIRE(v == true);
                      });

    auto p3 = p2.then([&](bool v) {
                          REQUIRE(v == true);
                          return ph.createResolved(false);
                      });

    auto p4 = p3.then([](bool v) {
                          REQUIRE(v == false);
                      });


    ioContext.run();
}
