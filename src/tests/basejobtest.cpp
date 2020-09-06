
#include <catch2/catch.hpp>

#include <iostream>
#include <future>

#include <job/basejob.hpp>
#include <job/cprjobhandler.hpp>
#include "tests.hpp"

using namespace Kazv;

TEST_CASE("Base job should fetch correctly", "[basejob]")
{
    boost::asio::io_context ioContext;
    BaseJob job(TEST_SERVER_URL, "/.well-known/matrix/client", BaseJob::Get{});

    CprJobHandler h(ioContext.get_executor());
    h.fetch(
        job,
        [](auto r) {
            if (r.statusCode == 200) {
                REQUIRE( BaseJob::isBodyJson(r.body) );

                json j = std::get<BaseJob::JsonBody>(r.body).get();

                REQUIRE_NOTHROW( (j["m.homeserver"]["base_url"]) );
                REQUIRE( (j["m.homeserver"]["base_url"].size() > 0) );
            }
        });
    ioContext.run();
}
