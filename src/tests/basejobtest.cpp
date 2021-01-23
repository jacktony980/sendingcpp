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

#include <libkazv-config.hpp>


#include <catch2/catch.hpp>

#include <iostream>
#include <future>

#include <basejob.hpp>
#include <cprjobhandler.hpp>
#include "tests.hpp"

using namespace Kazv;

TEST_CASE("Base job should fetch correctly", "[basejob]")
{
    boost::asio::io_context ioContext;
    BaseJob job(TEST_SERVER_URL, "/.well-known/matrix/client", BaseJob::Get{}, "TestJob");

    CprJobHandler h(ioContext.get_executor());
    h.submit(
        job.withData(json{{"test", "bar"}}),
        [&h](auto r) {
            if (r.statusCode == 200) {
                REQUIRE( isBodyJson(r.body) );

                json j = r.jsonBody().get();

                REQUIRE_NOTHROW( (j["m.homeserver"]["base_url"]) );
                REQUIRE( (j["m.homeserver"]["base_url"].size() > 0) );

                REQUIRE( r.dataStr("test") == "bar" );
            }
            h.stop();
        });
    ioContext.run();
}
