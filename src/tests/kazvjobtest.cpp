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
#include <iostream>

#include <catch2/catch.hpp>
#include <cprjobhandler.hpp>

#include "tests.hpp"

using namespace Kazv;

TEST_CASE("setTimeout should behave properly", "[kazvjob]")
{
    boost::asio::io_context ioContext;
    CprJobHandler h(ioContext.get_executor());

    std::vector<int> v;

    h.setTimeout(
        [&v, &h] {
            v.push_back(500);
            h.stop();
        }, 500);

    h.setTimeout(
        [&v] {
            v.push_back(100);
        }, 100);

    ioContext.run();

    REQUIRE( v.size() == 2 );
    REQUIRE( v[0] == 100 );
    REQUIRE( v[1] == 500 );
}

TEST_CASE("setInterval should behave properly", "[kazvjob]")
{
    boost::asio::io_context ioContext;
    CprJobHandler h(ioContext.get_executor());

    std::vector<int> v;

    h.setInterval(
        [&v, &h] {
            v.push_back(50);
            std::cout << "timer executed" << std::endl;
            if (v.size() >= 2) {
                h.stop();
            }
        }, 100);

    ioContext.run();

    REQUIRE( v.size() == 2 );
    REQUIRE( v[0] == 50 );
    REQUIRE( v[1] == 50 );
}

TEST_CASE("setInterval can be cancelled", "[kazvjob]")
{
    boost::asio::io_context ioContext;
    CprJobHandler h(ioContext.get_executor());

    std::vector<int> v;

    std::string id{"testTimerId"};

    h.setInterval(
        [&v, &h, id] {
            v.push_back(50);
            std::cout << "timer executed" << std::endl;
            if (v.size() >= 2) {
                h.cancel(id);
            }
        }, 100, id);

    h.setTimeout(
        [&h] {
            h.stop();
        }, 300);

    ioContext.run();

    REQUIRE( v.size() == 2 );
    REQUIRE( v[0] == 50 );
    REQUIRE( v[1] == 50 );
}

static BaseJob succJob =
    BaseJob(TEST_SERVER_URL, "/.well-known/matrix/client", BaseJob::Get{}, "TestJob")
    .withQueue("testjob");

static BaseJob failJob =
    BaseJob(TEST_SERVER_URL, "/.well-known/jfasjewfn/awejioaewjgjaad/fawre", BaseJob::Get{}, "AnotherTestJob")
    .withQueue("testjob");

static BaseJob failJobWithCancel =
    failJob.withQueue("testjob", CancelFutureIfFailed);

TEST_CASE("Job queue should behave properly", "[kazvjob]")
{
    std::vector<bool> v;

    std::function<void(Response)> callback =
        [&v](Response r) {
            std::cout << "callback called" << std::endl;
            v.push_back(r.success());
        };

    boost::asio::io_context ioContext;
    CprJobHandler h(ioContext.get_executor());

    h.submit(succJob, callback); // true
    h.submit(failJob, callback); // false
    h.submit(failJobWithCancel, callback); // false
    h.submit(succJob, callback); // false (because this is cancelled)
    h.submit(succJob, [&h](Response r) {
                          h.stop();
                      }); // nothing added

    ioContext.run();

    REQUIRE( v.size() == 4 );
    REQUIRE( v[0] == true );
    REQUIRE( v[1] == false );
    REQUIRE( v[2] == false );
    REQUIRE( v[3] == false );
}
