/*
 * Copyright (C) 2020-2021 Tusooa Zhu <tusooa@kazv.moe>
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
    /**
     * Represent a Matrix room.
     */
    class Room
    {
    public:
        using PromiseT = SingleTypePromise<DefaultRetType>;
        /**
         * Constructor.
         *
         * Construct the room with @c roomId .
         *
         */
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

        /**
         * Get whether this room is encrypted.
         *
         * The encryption status is changed to true if the client
         * receives a state event that turns on encryption.
         * If that state event is removed later, the status will
         * not be changed.
         *
         * @return A lager::reader<bool> that contains
         * whether this room is encrypted.
         */
        lager::reader<bool> encrypted() const;

        /*lager::reader<std::string>*/
        KAZV_WRAP_ATTR(RoomModel, m_room, roomId);
        /*lager::reader<RoomMembership>*/
        KAZV_WRAP_ATTR(RoomModel, m_room, membership);
        /*lager::reader<std::string>*/
        KAZV_WRAP_ATTR(RoomModel, m_room, localDraft);
        /* lager::reader<bool> */
        KAZV_WRAP_ATTR(RoomModel, m_room, membersFullyLoaded);

        /**
         * Set local draft for this room.
         *
         * After the returned Promise is resolved,
         * @c localDraft() will contain @c localDraft .
         *
         * @param localDraft The local draft to send.
         * @return A Promise that resolves when the local draft
         * has been set, or when there is an error.
         */
        PromiseT setLocalDraft(std::string localDraft) const;

        /**
         * Send an event to this room.
         *
         * @param msg The message to send
         * @return A Promise that resolves when the event has been sent,
         * or when there is an error.
         */
        PromiseT sendMessage(Event msg) const;

        /**
         * Send a text message to this room.
         *
         * @param text The text
         * @return A Promise that resolves when the text message has
         * been sent, or when there is an error.
         */
        PromiseT sendTextMessage(std::string text) const;

        /**
         * Get the full state of this room.
         *
         * This method will update the Client as needed.
         *
         * After the returned Promise resolves successfully,
         * @c stateEvents() will contain the fetched state.
         *
         * @return A Promise that resolves when the room state
         * has been fetched, or when there is an error.
         */
        PromiseT refreshRoomState() const;

        /**
         * Get one state event with @c type and @c stateKey .
         *
         * This method will update the Client as needed.
         *
         * After the returned Promise resolves successfully,
         * @c state({type,stateKey}) will contain the fetched
         * state event.
         *
         * @return A Promise that resolves when the state
         * event has been fetched, or when there is an error.
         */
        PromiseT getStateEvent(std::string type, std::string stateKey) const;

        /**
         * Send a state event to this room.
         *
         * @param state The state event to send.
         * @return A Promise that resolves when the state event
         * has been sent, or when there is an error.
         */
        PromiseT sendStateEvent(Event state) const;

        /**
         * Set the room name.
         *
         * @param name The new name for this room.
         * @return A Promise that resolves when the state event
         * for the name change has been sent, or when there is an error.
         */
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

        /**
         * Set the room topic.
         *
         * @param topic The new topic for this room.
         * @return A Promise that resolves when the state event
         * for the topic change has been sent, or when there is an error.
         */
        PromiseT setTopic(std::string topic) const;

        /**
         * Invite a user to this room
         *
         * @param userId The user id for the user to invite.
         * @return A Promise that resolves when the state event
         * for the invite has been sent, or when there is an error.
         */
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

        /**
         * Set the typing status of the current user in this room.
         *
         * @param typing Whether the user is now typing.
         * @param timeoutMs How long this typing status should last,
         * in milliseconds.
         * @return A Promise that resolves when the typing status
         * has been sent, or when there is an error.
         */
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

        /**
         * Leave this room.
         *
         * @return A Promise that resolves when the state event
         * for the leaving has been sent, or when there is an error.
         */
        PromiseT leave() const;

        /**
         * Forget this room.
         *
         * One can only forget a room when they have already left it.
         *
         * @return A Promise that resolves when the room has been
         * forgot, or when there is an error.
         */
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

        /**
         * Set pinned events of this room
         *
         * @param eventIds The event ids of the new pinned events
         *
         * @return A Promise that resolves when the state event
         * for the pinned events change has been sent, or when there is an error.
         */
        PromiseT setPinnedEvents(immer::flex_vector<std::string> eventIds) const;

        /**
         * Get the Gaps in the timeline for this room.
         *
         * Any key of the map in the returned reader can be send as
         * an argument of paginateBackFromEvent() to try to fill the Gap
         * at that event.
         *
         * @return A lager::reader that contains an evnetId-to-prevBatch map.
         */
        lager::reader<immer::map<std::string /* eventId */, std::string /* prevBatch */>> timelineGaps() const;

        /**
         * Try to paginate back from @c eventId.
         *
         * @param eventId An event id that is in the key of `+timelineGaps()`.
         *
         * @return A Promise that resolves when the pagination is
         * successful, or when there is an error. If it is successful,
         * `+timelineGaps()` will no longer contain eventId as key, and
         * `timeline()` will contain the events before eventId in the
         * full event chain on the homeserver.
         * If `eventId` is not in `+timelineGaps()`, it is considered
         * to be failed.
         */
        PromiseT paginateBackFromEvent(std::string eventId) const;

    private:
        lager::reader<SdkModel> m_sdk;
        lager::reader<RoomModel> m_room;
        Context<ClientAction> m_ctx;
    };
}
