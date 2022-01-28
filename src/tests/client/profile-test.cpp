/*
 * This file is part of libkazv.
 * SPDX-FileCopyrightText: 2022 Tusooa Zhu <tusooa@kazv.moe>
 * SPDX-License-Identifier: AGPL-3.0-or-later
 */

#include <libkazv-config.hpp>

#include <catch2/catch.hpp>

#include <boost/asio.hpp>

#include <asio-promise-handler.hpp>
#include <cursorutil.hpp>
#include <sdk-model.hpp>
#include <client/client.hpp>

#include "client-test-util.hpp"

static const json getUserProfileResponseJson = R"({
  "avatar_url": "mxc://matrix.org/SDGdghriugerRg",
  "displayname": "Alice Margatroid"
})"_json;

TEST_CASE("GetUserProfile", "[client][profile]")
{
    WHEN("We initiate this job")
    {
        ClientModel loggedInModel = createTestClientModel();
        auto [resModel, dontCareEffect] = ClientModel::update(loggedInModel, GetUserProfileAction{"@alice:example.com"});
        THEN("it should be added")
        {
            REQUIRE(resModel.nextJobs.size() == 1);
            REQUIRE(resModel.nextJobs[0].jobId() == "GetUserProfile");
        }

        THEN("we should not send access token")
        {
            REQUIRE_FALSE(hasAccessToken(resModel.nextJobs[0]));
        }
    }

    using namespace Kazv::CursorOp;

    boost::asio::io_context io;
    AsioPromiseHandler ph{io.get_executor()};

    auto store = createTestClientStore(ph);

    WHEN("We got a successful response")
    {
        auto resp = createResponse("GetUserProfile", getUserProfileResponseJson);

        THEN("We should return the avatar url and display name in the response")
        {
            store.dispatch(ProcessResponseAction{resp})
                .then([](auto stat) {
                    REQUIRE(stat.success());
                    REQUIRE(stat.dataStr("avatarUrl") == getUserProfileResponseJson["avatar_url"]);
                    REQUIRE(stat.dataStr("displayName") == getUserProfileResponseJson["displayname"]);
                });
        }
    }

    WHEN("We got a failed response")
    {
        auto resp = createResponse("GetUserProfile", json{});
        resp.statusCode = 404;

        THEN("We should fail")
        {
            store.dispatch(ProcessResponseAction{resp})
                .then([](auto stat) {
                    REQUIRE(!stat.success());
                    REQUIRE(stat.dataStr("errorCode") == "404");
                });
        }
    }

    io.run();
}
