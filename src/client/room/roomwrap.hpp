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
#include <lager/with.hpp>
#include <lager/lenses/optional.hpp>
#include <zug/transducer/map.hpp>
#include <zug/transducer/filter.hpp>
#include <zug/sequence.hpp>
#include <immer/flex_vector_transient.hpp>

#include "room.hpp"
#include "client/cursorutil.hpp"

namespace Kazv
{
    class RoomWrap
    {
    public:
        inline RoomWrap(lager::reader<Room> room, lager::context<Client::Action> ctx)
            : m_room(room)
            , m_ctx(ctx) {}

        /* lager::reader<MapT<KeyOfState, Event>> */
        inline auto stateEvents() const {
            return m_room
                [&Room::stateEvents];
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
            return m_room
                [&Room::stateEvents]
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
                [&Room::stateEvents]
                .xform(zug::map(
                           [=](auto eventMap) {
                               return intoImmer(
                                   immer::flex_vector<std::string>{},
                                   memberNameTransducer,
                                   eventMap);
                           }));

        }

        lager::reader<bool> encrypted() const;

        void sendMessage(Event msg) const;

        void sendStateEvent(Event state) const;

    private:
        lager::reader<Room> m_room;
        lager::context<Client::Action> m_ctx;
    };
}
