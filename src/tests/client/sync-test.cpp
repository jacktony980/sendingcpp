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


#include <catch2/catch.hpp>

#include <zug/into_vector.hpp>

#include <client/cursorutil.hpp>
#include <client/client.hpp>

#include "client-test-util.hpp"

// The example response is adapted from https://matrix.org/docs/spec/client_server/latest
static json syncResponseJson = R"({
  "next_batch": "s72595_4483_1934",
  "presence": {
    "events": [
      {
        "content": {
          "avatar_url": "mxc://localhost:wefuiwegh8742w",
          "last_active_ago": 2478593,
          "presence": "online",
          "currently_active": false,
          "status_msg": "Making cupcakes"
        },
        "type": "m.presence",
        "sender": "@example:localhost"
      }
    ]
  },
  "account_data": {
    "events": [
      {
        "type": "org.example.custom.config",
        "content": {
          "custom_config_key": "custom_config_value"
        }
      }
    ]
  },
  "rooms": {
    "join": {
      "!726s6s6q:example.com": {
        "summary": {
          "m.heroes": [
            "@alice:example.com",
            "@bob:example.com"
          ],
          "m.joined_member_count": 2,
          "m.invited_member_count": 0
        },
        "state": {
          "events": [
            {
              "content": {
                "membership": "join",
                "avatar_url": "mxc://example.org/SEsfnsuifSDFSSEF",
                "displayname": "Alice Margatroid"
              },
              "type": "m.room.member",
              "event_id": "$143273582443PhrSn:example.org",
              "room_id": "!726s6s6q:example.com",
              "sender": "@example:example.org",
              "origin_server_ts": 1432735824653,
              "unsigned": {
                "age": 1234
              },
              "state_key": "@alice:example.org"
            }
          ]
        },
        "timeline": {
          "events": [
            {
              "content": {
                "membership": "join",
                "avatar_url": "mxc://example.org/SEsfnsuifSDFSSEF",
                "displayname": "Alice Margatroid"
              },
              "type": "m.room.member",
              "event_id": "$143273582443PhrSn:example.org",
              "room_id": "!726s6s6q:example.com",
              "sender": "@example:example.org",
              "origin_server_ts": 1432735824653,
              "unsigned": {
                "age": 1234
              },
              "state_key": "@alice:example.org"
            },
            {
              "content": {
                "body": "This is an example text message",
                "msgtype": "m.text",
                "format": "org.matrix.custom.html",
                "formatted_body": "<b>This is an example text message</b>"
              },
              "type": "m.room.message",
              "event_id": "$anothermessageevent:example.org",
              "room_id": "!726s6s6q:example.com",
              "sender": "@example:example.org",
              "origin_server_ts": 1432735824653,
              "unsigned": {
                "age": 1234
              }
            }
          ],
          "limited": true,
          "prev_batch": "t34-23535_0_0"
        },
        "ephemeral": {
          "events": [
            {
              "content": {
                "user_ids": [
                  "@alice:matrix.org",
                  "@bob:example.com"
                ]
              },
              "type": "m.typing",
              "room_id": "!jEsUZKDJdhlrceRyVU:example.org"
            }
          ]
        },
        "account_data": {
          "events": [
            {
              "content": {
                "tags": {
                  "u.work": {
                    "order": 0.9
                  }
                }
              },
              "type": "m.tag"
            },
            {
              "type": "org.example.custom.room.config",
              "content": {
                "custom_config_key": "custom_config_value"
              }
            },
            {
              "type": "m.fully_read",
              "content": {
                "event_id": "$anothermessageevent:example.org"
              }
            }
          ]
        }
      }
    },
    "invite": {
      "!696r7674:example.com": {
        "invite_state": {
          "events": [
            {
              "sender": "@alice:example.com",
              "type": "m.room.name",
              "state_key": "",
              "content": {
                "name": "My Room Name"
              }
            },
            {
              "sender": "@alice:example.com",
              "type": "m.room.member",
              "state_key": "@bob:example.com",
              "content": {
                "membership": "invite"
              }
            }
          ]
        }
      }
    },
    "leave": {}
  },
  "to_device": {
    "events": [
      {
        "sender": "@alice:example.com",
        "type": "m.new_device",
        "content": {
          "device_id": "XYZABCDE",
          "rooms": ["!726s6s6q:example.com"]
        }
      }
    ]
  }
})"_json;

TEST_CASE("use sync response to update client model", "[client][sync]")
{
    using namespace Kazv::CursorOp;

    auto store = createTestClientStore();

    auto resp = createResponse("Sync", syncResponseJson);

    auto client = Client(store, store);

    store.dispatch(ProcessResponseAction{resp});

    auto rooms = +client.rooms();

    std::string roomId = "!726s6s6q:example.com";

    SECTION("rooms should be added") {
        REQUIRE(rooms.find(roomId));
    }

    auto r = client.room(roomId);

    SECTION("room members should be updated") {
        auto members = +r.members();

        auto hasAlice = zug::into_vector(
            zug::filter([](auto id) { return id == "@alice:example.org"; }),
            members)
            .size() > 0;

        REQUIRE(hasAlice);
    }

    SECTION("ephemeral events should be updated") {
        auto users = +r.typingUsers();

        REQUIRE((users == immer::flex_vector<std::string>{
                    "@alice:matrix.org",
                    "@bob:example.com"
                }));
    }

    auto eventId = "$anothermessageevent:example.org"s;

    SECTION("timeline should be updated") {
        auto timeline = +r.timelineEvents();

        auto filtered = zug::into_vector(
            zug::filter([=](auto event) { return event.id() == eventId; }),
            timeline);

        auto hasEvent = filtered.size() > 0;
        REQUIRE(hasEvent);

        auto onlyOneEvent = filtered.size() == 1;
        REQUIRE(onlyOneEvent);

        auto ev = filtered[0];

        auto eventHasRoomId = ev.originalJson().get().contains("room_id"s);
        REQUIRE(eventHasRoomId);
    }

    SECTION("fully read marker should be updated") {
        auto readMarker = +r.readMarker();

        REQUIRE(readMarker == eventId);
    }

    SECTION("toDevice should be updated") {
        auto toDevice = +client.toDevice();

        REQUIRE(toDevice.size() == 1);
        REQUIRE(toDevice[0].sender() == "@alice:example.com");
    }
}
