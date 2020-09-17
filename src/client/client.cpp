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


#include <lager/util.hpp>
#include <lager/context.hpp>
#include <functional>
// eager requires tuplify but did not include it.
// This is a bug.
#include <zug/tuplify.hpp>
#include <zug/transducer/eager.hpp>
#include <zug/into.hpp>
#include <immer/flex_vector_transient.hpp>

#include "client.hpp"
#include "csapi/login.hpp"
#include "types.hpp"
#include "debug.hpp"
#include "job/jobinterface.hpp"
#include "eventemitter/eventinterface.hpp"
#include "client/util.hpp"

namespace Kazv
{
    Client::Effect loginEffect(Client::LoginAction a)
    {
        return
            [=](auto &&ctx) {
                LoginJob job(a.serverUrl,
                             "m.login.password"s, // type
                             UserIdentifier{ "m.id.user"s, json{{"user", a.username}} }, // identifier
                             a.password,
                             {}, // token, not used
                             {}, // device id, not used
                             a.deviceName.value_or("libkazv"));
                auto &jobHandler = lager::get<JobInterface &>(ctx);
                jobHandler.fetch(
                    job,
                    [=](BaseJob::Response r) {
                        if (LoginJob::success(r)) {
                            dbgClient << "Job success" << std::endl;
                            const json &j = jsonBody(r).get();
                            std::string serverUrl = j.contains("well_known")
                                ? j.at("well_known").at("m.homeserver").at("base_url").get<std::string>()
                                : a.serverUrl;
                            ctx.dispatch(Client::LoadUserInfoAction{
                                    serverUrl,
                                    j.at("user_id"),
                                    j.at("access_token"),
                                    j.at("device_id"),
                                    /* loggedIn = */ true
                                });
                        }
                    });
            };
    }

    lager::effect<Client::Action> logoutEffect(Client::LogoutAction)
    {
        return
            [=](auto &&ctx) {
                ctx.dispatch(Client::LoadUserInfoAction{
                        ""s,
                        ""s,
                        ""s,
                        ""s,
                        /* loggedIn = */ true
                    });
            };
    }

    static void loadRoomsFromSyncInPlace(Client &m, SyncJob::Rooms rooms)
    {
        auto l = m.roomList;

        auto updateRoomImpl =
            [&l](auto id, auto a) {
                l = RoomList::update(
                    std::move(l),
                    RoomList::UpdateRoomAction{id, a});
            };
        auto updateSingleRoom =
            [updateRoomImpl](auto id, auto room, auto membership) {
                updateRoomImpl(id, Room::ChangeMembershipAction{membership});
                updateRoomImpl(id, Room::AppendTimelineAction{room.timeline.events});
                if (room.state) {
                    updateRoomImpl(id, Room::AddStateEventsAction{room.state.value().events});
                }
                if (room.accountData) {
                    updateRoomImpl(id, Room::AddAccountDataAction{room.accountData.value().events});
                }
            };

        auto updateJoinedRoom =
            [=](auto id, auto room) {
                updateSingleRoom(id, room, Room::Membership::Join);
            };

        auto updateLeftRoom =
            [=](auto id, auto room) {
                updateSingleRoom(id, room, Room::Membership::Leave);
            };

        for (auto &&[id, room]: rooms.join) {
            updateJoinedRoom(id, room);
            // TODO update other info such as
            // ephemeral, notification and summary
        }

        // TODO update info for invited rooms

        for (auto &&[id, room]: rooms.leave) {
            updateLeftRoom(id, room);
        }

        m.roomList = l;
    }

    static void loadPresenceFromSyncInPlace(Client &m, EventList presence)
    {
        m.presence = merge(std::move(m.presence), presence, keyOfPresence);
    }

    static void loadAccountDataFromSyncInPlace(Client &m, EventList accountData)
    {
        m.accountData = merge(std::move(m.accountData), accountData, keyOfAccountData);
    }

    auto Client::update(Client m, Action a) -> Result
    {
        return lager::match(std::move(a))(
            [=](Error::Action a) mutable -> Result {
                m.error = Error::update(m.error, a);
                return {std::move(m), lager::noop};
            },

            [=](RoomList::Action a) mutable -> Result {
                m.roomList = RoomList::update(std::move(m.roomList), a);
                return {std::move(m), lager::noop};
            },

            [=](LoginAction a) mutable -> Result {
                return {std::move(m), loginEffect(std::move(a))};
            },

            [=](LogoutAction a) mutable -> Result {
                return {std::move(m), logoutEffect(std::move(a))};
            },

            [=](SyncAction a) -> Result {
                return {m, syncEffect(m, a)};
            },

            [=](LoadUserInfoAction a) mutable -> Result {
                dbgClient << "LoadUserInfoAction: " << a.userId << std::endl;

                m.serverUrl = a.serverUrl;
                m.userId = a.userId;
                m.token = a.token;
                m.deviceId = a.deviceId;
                m.loggedIn = a.loggedIn;

                return { std::move(m),
                         [](auto &&ctx) {
                             auto &eventEmitter = lager::get<EventInterface &>(ctx);
                             eventEmitter.emit(LoginSuccessful{});
                             ctx.dispatch(Error::SetErrorAction{Error::NoError{}});
                             // after user info is loaded, do first sync
                             ctx.dispatch(Client::SyncAction{});
                         }
                };
            },

            [=](LoadSyncResultAction a) mutable -> Result {
                m.syncToken = a.syncToken;
                if (a.rooms) {
                    loadRoomsFromSyncInPlace(m, a.rooms.value());
                }

                if (a.presence) {
                    loadPresenceFromSyncInPlace(m, a.presence.value().events);
                }

                if (a.accountData) {
                    loadAccountDataFromSyncInPlace(m, a.accountData.value().events);
                }

                return { std::move(m), lager::noop };
            },

            [&](Client::PaginateTimelineAction a) -> Result {
                auto eff = paginateTimelineEffect(m, a);
                return { std::move(m), eff };
            },
            [&](Client::LoadPaginateTimelineResultAction a) -> Result {
                try {
                    auto room = m.roomList.at(a.roomId);
                    immer::flex_vector_transient<Event> events{};
                    // The timeline from paginate backwards is returned
                    // in reversed order, so restore the order.
                    zug::into(events, zug::reversed, a.events);

                    Room::PrependTimelineAction action
                        {events.persistent(), a.paginateBackToken};

                    auto roomList = RoomList::update(
                        std::move(m.roomList),
                        RoomList::UpdateRoomAction{a.roomId, action});
                    m.roomList = std::move(roomList);

                    return { std::move(m), lager::noop };
                } catch (const std::out_of_range &e) {
                    // Ignore it, the client model is modified
                    // such that it knows nothing about this room.
                    // May happen in debugger.
                    return { std::move(m), lager::noop };
                }
            },

            [&](SendMessageAction a) -> Result {
                // FIXME: This implementation is questionable
                // as it causes problem for time travel:
                // after you rolled back, the rolled-back txnIds
                // are not usable again, and the first few messages
                // will not be sent successfully.
                // Maybe one wants to use a dependency to store
                // this info, or to use a hash plus the timestamp
                // as the txnId.
                auto eff = sendMessageEffect(m, a);
                m.nextTxnId = increaseTxnId(m.nextTxnId);
                return {std::move(m), eff};
            }
            );

    }

    std::string Client::increaseTxnId(std::string cur)
    {
        return std::to_string(std::stoull(cur) + 1);
    }
}
