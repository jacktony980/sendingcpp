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

#pragma once

#include <lager/reader.hpp>
#include <lager/context.hpp>
#include <lager/with.hpp>
#include <lager/lenses/optional.hpp>
#include <zug/transducer/map.hpp>
#include <zug/transducer/filter.hpp>
#include <zug/sequence.hpp>
#include <immer/flex_vector_transient.hpp>

#include "client-model.hpp"
#include "room-model.hpp"
#include "client/cursorutil.hpp"

namespace Kazv
{
    class Room
    {
    public:
        inline Room(lager::reader<RoomModel> room, lager::context<ClientAction> ctx)
            : m_room(room)
            , m_ctx(ctx) {}

        /* lager::reader<MapT<KeyOfState, Event>> */
        inline auto stateEvents() const {
            return m_room
                [&RoomModel::stateEvents];
        }

        /* lager::reader<std::optional<Event>> */
        inline auto stateOpt(KeyOfState k) const {
            return stateEvents()
                [std::move(k)];
        }

        /* lager::reader<Event> */
        inline auto state(KeyOfState k) const {
            return stateOpt(k)
                [lager::lenses::or_default];
        }

        /* lager::reader<RangeT<Event>> */
        inline auto timelineEvents() const {
            return m_room
                .xform(zug::map([](auto r) {
                                    auto messages = r.messages;
                                    auto timeline = r.timeline;
                                    return intoImmer(
                                        immer::flex_vector<Event>{},
                                        zug::map([=](auto eventId) {
                                                     return messages[eventId];
                                                 }),
                                        timeline);
                                }));
        }

        /* lager::reader<std::string> */
        inline auto name() const {
            using namespace lager::lenses;
            return stateEvents()
                [KeyOfState{"m.room.name", ""}]
                [or_default]
                .xform(zug::map([](Event ev) {
                                    auto content = ev.content().get();
                                    return
                                        content.contains("name")
                                        ? std::string(content["name"])
                                        // TODO: use heroes to generate a name
                                        : "<no name>";
                                }));
        }

        /* lager::reader<RangeT<std::string>> */
        inline auto members() const {
            using MemberNode = std::pair<std::string, Kazv::Event>;
            auto memberNameTransducer =
                zug::filter(
                    [](auto val) {
                        auto [k, v] = val;
                        auto [type, stateKey] = k;
                        return type == "m.room.member"s;
                    })
                | zug::map(
                    [](auto val) {
                        auto [k, v] = val;
                        auto [type, stateKey] = k;
                        return MemberNode{stateKey, v};
                    })
                | zug::filter(
                    [](auto val) {
                        auto [stateKey, ev] = val;
                        return ev.content().get()
                            .at("membership"s) == "join"s;
                    })
                | zug::map(
                    [](auto val) {
                        auto [stateKey, ev] = val;
                        return stateKey;
                    });

            return m_room
                [&RoomModel::stateEvents]
                .xform(zug::map(
                           [=](auto eventMap) {
                               return intoImmer(
                                   immer::flex_vector<std::string>{},
                                   memberNameTransducer,
                                   eventMap);
                           }));

        }

        /* lager::reader<std::optional<Event>> */
        inline auto memberEventFor(std::string userId) const {
            return m_room
                [&RoomModel::stateEvents]
                .xform(containerMap(immer::flex_vector<Event>{},
                                    zug::filter([=](auto val) {
                                                    auto [k, v] = val;
                                                    auto [type, stateKey] = k;
                                                    return type == "m.room.member"s && stateKey == userId;
                                                }) // -> RangeT<pair<KeyofState{...}, Event>>
                                    | zug::map([](auto val) {
                                                   auto [k, event] = val;
                                                   return event;
                                               })))
                [0];
        }

        lager::reader<bool> encrypted() const;

        /*lager::reader<std::string>*/
        KAZV_WRAP_ATTR(RoomModel, m_room, roomId);
        /*lager::reader<RoomMembership>*/
        KAZV_WRAP_ATTR(RoomModel, m_room, membership);

        inline void sendMessage(Event msg) const {
            using namespace CursorOp;
            m_ctx.dispatch(SendMessageAction{+roomId(), msg});
        }

        inline void sendTextMessage(std::string text) const {
            json j{
                {"type", "m.room.message"},
                {"content", {
                        {"msgtype", "m.text"},
                        {"body", text}
                    }
                }
            };
            Event e{j};
            sendMessage(e);
        }

        inline void sendStateEvent(Event state) const {
            using namespace CursorOp;
            m_ctx.dispatch(SendStateEventAction{+roomId(), state});
        }

        inline void setName(std::string name) const {
            json j{
                {"type", "m.room.name"},
                {"content", {
                        {"name", name}
                    }
                }
            };
            Event e{j};
            sendStateEvent(e);
        }

        // lager::reader<std::string>
        inline auto topic() const {
            using namespace lager::lenses;
            return stateEvents()
                [KeyOfState{"m.room.topic", ""}]
                [or_default]
                .xform(eventContent
                       | jsonAtOr("topic"s, ""s));
        }

        inline void setTopic(std::string topic) const {
            json j{
                {"type", "m.room.topic"},
                {"content", {
                        {"topic", topic}
                    }
                }
            };
            Event e{j};
            sendStateEvent(e);
        }

        inline void invite(std::string userId) const {
            using namespace CursorOp;
            m_ctx.dispatch(InviteToRoomAction{+roomId(), userId});
        }

        /* lager::reader<MapT<std::string, Event>> */
        inline auto ephemeralEvents() const {
            return m_room
                [&RoomModel::ephemeral];
        }

        /* lager::reader<std::optional<Event>> */
        inline auto ephemeralOpt(std::string type) const {
            return m_room
                [&RoomModel::ephemeral]
                [type];
        }

        /* lager::reader<Event> */
        inline auto ephemeral(std::string type) const {
            return m_room
                [&RoomModel::ephemeral]
                [type]
                [lager::lenses::or_default];
        }

        /* lager::reader<RangeT<std::string>> */
        inline auto typingUsers() const {
            using namespace lager::lenses;
            return ephemeral("m.typing")
                .xform(eventContent
                       | jsonAtOr("user_ids",
                                  immer::flex_vector<std::string>{}));
        }

        inline void setTyping(bool typing, std::optional<int> timeoutMs) const {
            using namespace CursorOp;
            m_ctx.dispatch(SetTypingAction{+roomId(), typing, timeoutMs});
        }

        /* lager::reader<MapT<std::string, Event>> */
        inline auto accountDataEvents() const {
            return m_room
                [&RoomModel::accountData];
        }

        /* lager::reader<std::optional<Event>> */
        inline auto accountDataOpt(std::string type) const {
            return m_room
                [&RoomModel::accountData]
                [type];
        }

        /* lager::reader<Event> */
        inline auto accountData(std::string type) const {
            return m_room
                [&RoomModel::accountData]
                [type]
                [lager::lenses::or_default];
        }

        /* lager::reader<std::string> */
        inline auto readMarker() const {
            using namespace lager::lenses;
            return accountData("m.fully_read")
                .xform(eventContent
                       | jsonAtOr("event_id", std::string{}));
        }

        inline void leave() const {
            using namespace CursorOp;
            m_ctx.dispatch(LeaveRoomAction{+roomId()});
        }

        inline void forget() const {
            using namespace CursorOp;
            m_ctx.dispatch(ForgetRoomAction{+roomId()});
        }

        /* lager::reader<JsonWrap> */
        inline auto avatar() const {
            return state(KeyOfState{"m.room.avatar", ""})
                .xform(eventContent);
        }

        /* lager::reader<RangeT<std::string>> */
        inline auto pinnedEvents() const {
            return state(KeyOfState{"m.room.pinned_events", ""})
                .xform(eventContent
                       | jsonAtOr("pinned", immer::flex_vector<std::string>{}));
        }

        inline void setPinnedEvents(immer::flex_vector<std::string> eventIds) const {
            json j{
                {"type", "m.room.pinned_events"},
                {"content", {
                        {"pinned", eventIds}
                    }
                }
            };
            Event e{j};
            sendStateEvent(e);
        }

    private:
        lager::reader<RoomModel> m_room;
        lager::context<ClientAction> m_ctx;
    };
}
