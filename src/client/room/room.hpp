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
#include <libkazv-config.hpp>

#include <lager/reader.hpp>
#include <lager/context.hpp>
#include <lager/with.hpp>
#include <lager/constant.hpp>
#include <lager/lenses/optional.hpp>
#include <zug/transducer/map.hpp>
#include <zug/transducer/filter.hpp>
#include <zug/sequence.hpp>
#include <immer/flex_vector_transient.hpp>

#include "debug.hpp"

#include "sdk-model.hpp"
#include "client-model.hpp"
#include "room-model.hpp"
#include <cursorutil.hpp>

namespace Kazv
{
    class Room
    {
    public:
        using PromiseT = SingleTypePromise<DefaultRetType>;
        Room(lager::reader<SdkModel> sdk,
             lager::reader<std::string> roomId,
             Context<ClientAction> ctx);

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

        /* lager::reader<std::string> */
        inline auto avatarMxcUri() const {
            using namespace lager::lenses;
            return stateEvents()
                [KeyOfState{"m.room.avatar", ""}]
                [or_default]
                .xform(zug::map([](Event ev) {
                                    auto content = ev.content().get();
                                    return
                                        content.contains("avatar")
                                        ? std::string(content["avatar"])
                                        : "";
                                }));
        }

        /* lager::reader<RangeT<std::string>> */
        inline auto members() const {
            return m_room.xform(zug::map([=](auto room) {
                                             return room.joinedMemberIds();
                                         }));
        }

        inline auto memberEventByCursor(lager::reader<std::string> userId) const {
            return lager::with(m_room[&RoomModel::stateEvents], userId)
                .xform(zug::map([](auto events, auto userId) {
                                    auto k = KeyOfState{"m.room.member", userId};
                                    return events[k];
                                }));
        }

        /* lager::reader<std::optional<Event>> */
        inline auto memberEventFor(std::string userId) const {
            return memberEventByCursor(lager::make_constant(userId));
        }

        lager::reader<bool> encrypted() const;

        /*lager::reader<std::string>*/
        KAZV_WRAP_ATTR(RoomModel, m_room, roomId);
        /*lager::reader<RoomMembership>*/
        KAZV_WRAP_ATTR(RoomModel, m_room, membership);
        /*lager::reader<std::string>*/
        KAZV_WRAP_ATTR(RoomModel, m_room, localDraft);
        /* lager::reader<bool> */
        KAZV_WRAP_ATTR(RoomModel, m_room, membersFullyLoaded);

        PromiseT setLocalDraft(std::string localDraft) const;

        PromiseT sendMessage(Event msg) const;

        PromiseT sendTextMessage(std::string text) const;

        PromiseT refreshRoomState() const;

        PromiseT getStateEvent(std::string type, std::string stateKey) const;

        PromiseT sendStateEvent(Event state) const;

        PromiseT setName(std::string name) const;

        // lager::reader<std::string>
        inline auto topic() const {
            using namespace lager::lenses;
            return stateEvents()
                [KeyOfState{"m.room.topic", ""}]
                [or_default]
                .xform(eventContent
                       | jsonAtOr("topic"s, ""s));
        }

        PromiseT setTopic(std::string topic) const;

        PromiseT invite(std::string userId) const;

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

        PromiseT setTyping(bool typing, std::optional<int> timeoutMs) const;

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

        PromiseT leave() const;

        PromiseT forget() const;

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

        PromiseT setPinnedEvents(immer::flex_vector<std::string> eventIds) const;
    private:
        lager::reader<SdkModel> m_sdk;
        lager::reader<RoomModel> m_room;
        Context<ClientAction> m_ctx;
    };
}
