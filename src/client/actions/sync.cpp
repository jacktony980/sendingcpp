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
#include <zug/transducer/map.hpp>

#include <jobinterface.hpp>
#include <debug.hpp>

#include "cursorutil.hpp"

#include "sync.hpp"


namespace Kazv
{
    // Atomicity guaranteed: if the sync action is created
    // before an action that reasonably changes Client
    // (e.g. roll back to an earlier state, obtain other
    // events), but executed
    // after that action, the sync will still give continuous
    // data about the events. (Sync will not "skip" events)
    // This is because this function takes the sync token
    // from the ClientModel model it is passed.
    ClientResult updateClient(ClientModel m, SyncAction)
    {
        kzo.client.dbg() << "Start syncing with token " << m.syncToken << std::endl;
        m.addJob(SyncJob(m.serverUrl,
                         m.token,
                         {}, // filter
                         m.syncToken));
        return { m, lager::noop };
    }


    static KazvEventList loadRoomsFromSyncInPlace(ClientModel &m, SyncJob::Rooms rooms)
    {
        auto l = m.roomList;
        auto eventsToEmit = KazvEventList{};

        auto updateRoomImpl =
            [&l](auto id, auto a) {
                l = RoomListModel::update(
                    std::move(l),
                    UpdateRoomAction{id, a});
            };
        auto updateSingleRoom =
            [&, updateRoomImpl](auto id, auto room, auto membership) {
                if (!l.has(id) || l[id].membership != membership) {
                    eventsToEmit = eventsToEmit.push_back(RoomMembershipChanged{membership, id});
                }
                updateRoomImpl(id, ChangeMembershipAction{membership});
                eventsToEmit = std::move(eventsToEmit) + intoImmer(
                    KazvEventList{},
                    zug::map([=](Event e) -> KazvEvent { return ReceivingRoomTimelineEvent{e, id}; }),
                    room.timeline.events);
                updateRoomImpl(id, AppendTimelineAction{room.timeline.events});
                if (room.state) {
                    eventsToEmit = std::move(eventsToEmit) + intoImmer(
                        KazvEventList{},
                        zug::map([=](Event e) -> KazvEvent { return ReceivingRoomStateEvent{e, id}; }),
                        room.state.value().events);
                    updateRoomImpl(id, AddStateEventsAction{room.state.value().events});
                }
                if (room.accountData) {
                    eventsToEmit = std::move(eventsToEmit) + intoImmer(
                        KazvEventList{},
                        zug::map([=](Event e) -> KazvEvent { return ReceivingRoomAccountDataEvent{e, id}; }),
                        room.state.value().events);
                    updateRoomImpl(id, AddAccountDataAction{room.accountData.value().events});
                }
            };

        auto updateJoinedRoom =
            [=](auto id, auto room) {
                updateSingleRoom(id, room, RoomMembership::Join);
                if (room.ephemeral) {
                    updateRoomImpl(id, AddEphemeralAction{room.ephemeral.value().events});
                }
                // TODO update other info such as
                // notification and summary
            };

        auto updateInvitedRoom =
            [=](auto id, auto room) {
                updateRoomImpl(id, ChangeMembershipAction{RoomMembership::Invite});
                if (room.inviteState) {
                    updateRoomImpl(id, ChangeInviteStateAction{room.inviteState.value().events});
                }
            };

        auto updateLeftRoom =
            [=](auto id, auto room) {
                updateSingleRoom(id, room, RoomMembership::Leave);
            };

        for (auto &&[id, room]: rooms.join) {
            updateJoinedRoom(id, room);
        }

        // TODO update info for invited rooms
        for (auto &&[id, room]: rooms.invite) {
            updateInvitedRoom(id, room);
        }

        for (auto &&[id, room]: rooms.leave) {
            updateLeftRoom(id, room);
        }

        m.roomList = l;
        return eventsToEmit;
    }

    static KazvEventList loadPresenceFromSyncInPlace(ClientModel &m, EventList presence)
    {
        auto eventsToEmit = intoImmer(
            KazvEventList{},
            zug::map([](Event e) { return ReceivingPresenceEvent{e}; }),
            presence);
        m.presence = merge(std::move(m.presence), presence, keyOfPresence);
        return eventsToEmit;
    }

    static KazvEventList loadAccountDataFromSyncInPlace(ClientModel &m, EventList accountData)
    {
        auto eventsToEmit = intoImmer(
            KazvEventList{},
            zug::map([](Event e) { return ReceivingPresenceEvent{e}; }),
            accountData);
        m.accountData = merge(std::move(m.accountData), accountData, keyOfAccountData);
        return eventsToEmit;
    }

    ClientResult processResponse(ClientModel m, SyncResponse r)
    {
        if (! r.success()) {
            m.addTrigger(SyncFailed{});
            kzo.client.dbg() << "Sync failed" << std::endl;
            kzo.client.dbg() << r.statusCode << std::endl;
            if (isBodyJson(r.body)) {
                auto j = r.jsonBody();
                kzo.client.dbg() << "Json says: " << j.get().dump() << std::endl;
            } else {
                kzo.client.dbg() << "Response body: "
                          << std::get<BaseJob::BytesBody>(r.body) << std::endl;
            }
            return { m, lager::noop };
        }

        kzo.client.dbg() << "Sync successful" << std::endl;

        auto rooms = r.rooms();
        auto accountData = r.accountData();
        auto presence = r.presence();
        // load the info that has been sync'd

        m.syncToken = r.nextBatch();

        m.addTrigger(SyncSuccessful{m.syncToken});

        if (rooms) {
            m.addTriggers(loadRoomsFromSyncInPlace(m, rooms.value()));
        }

        if (presence) {
            m.addTriggers(loadPresenceFromSyncInPlace(m, presence.value().events));
        }

        if (accountData) {
            m.addTriggers(loadAccountDataFromSyncInPlace(m, accountData.value().events));
        }

        // TODO: process toDevice, deviceLists, deviceOneTimeKeysCount

        return { std::move(m), lager::noop };
    }
}
