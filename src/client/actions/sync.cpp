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
        kzo.client.dbg() << "Start syncing with token " <<
            (m.syncToken ? m.syncToken.value() : "<null>") << std::endl;

        std::string filter = m.syncToken ? m.incrementalSyncFilterId : m.initialSyncFilterId;
        m.addJob(m.job<SyncJob>()
                 .make(filter, m.syncToken));
        return { m, lager::noop };
    }


    static KazvEventList loadRoomsFromSyncInPlace(ClientModel &m, SyncJob::Rooms rooms)
    {
        auto l = std::move(m.roomList);
        auto eventsToEmit = KazvEventList{}.transient();

        auto updateRoomImpl =
            [&l](auto id, auto a) {
                l = RoomListModel::update(
                    std::move(l),
                    UpdateRoomAction{std::move(id), std::move(a)});
            };
        auto updateSingleRoom =
            [&, updateRoomImpl](const auto &id, const auto &room, auto membership) {
                if (!l.has(id) || l[id].membership != membership) {
                    eventsToEmit.push_back(RoomMembershipChanged{membership, id});
                }
                updateRoomImpl(id, ChangeMembershipAction{membership});
                eventsToEmit.append(
                    intoImmer(
                        KazvEventList{},
                        zug::map([=](Event e) -> KazvEvent {
                                     return ReceivingRoomTimelineEvent{std::move(e), id};
                                 }),
                        room.timeline.events).transient());
                updateRoomImpl(id, AppendTimelineAction{room.timeline.events});
                if (room.state) {
                    eventsToEmit.append(
                        intoImmer(
                            KazvEventList{},
                            zug::map([=](Event e) -> KazvEvent {
                                         return ReceivingRoomStateEvent{std::move(e), id};
                                     }),
                            room.state.value().events).transient());
                    updateRoomImpl(id, AddStateEventsAction{room.state.value().events});
                }
                if (room.accountData) {
                    eventsToEmit.append(
                        intoImmer(
                            KazvEventList{},
                            zug::map([=](Event e) -> KazvEvent {
                                         return ReceivingRoomAccountDataEvent{std::move(e), id};
                                     }),
                            room.state.value().events).transient());
                    updateRoomImpl(id, AddAccountDataAction{room.accountData.value().events});
                }
            };

        auto updateJoinedRoom =
            [=](const auto &id, const auto &room) {
                updateSingleRoom(id, room, RoomMembership::Join);
                if (room.ephemeral) {
                    updateRoomImpl(id, AddEphemeralAction{room.ephemeral.value().events});
                }
                // TODO update other info such as
                // notification and summary
            };

        auto updateInvitedRoom =
            [=](const auto &id, const auto &room) {
                updateRoomImpl(id, ChangeMembershipAction{RoomMembership::Invite});
                if (room.inviteState) {
                    updateRoomImpl(id, ChangeInviteStateAction{room.inviteState.value().events});
                }
            };

        auto updateLeftRoom =
            [=](const auto &id, const auto &room) {
                updateSingleRoom(id, room, RoomMembership::Leave);
            };

        for (const auto &[id, room]: rooms.join) {
            updateJoinedRoom(id, room);
        }

        // TODO update info for invited rooms
        for (const auto &[id, room]: rooms.invite) {
            updateInvitedRoom(id, room);
        }

        for (const auto &[id, room]: rooms.leave) {
            updateLeftRoom(id, room);
        }

        m.roomList = std::move(l);
        return eventsToEmit.persistent();
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
            m.syncing = false;
            kzo.client.dbg() << "Sync failed" << std::endl;
            kzo.client.dbg() << r.statusCode << std::endl;
            if (isBodyJson(r.body)) {
                auto j = r.jsonBody();
                kzo.client.dbg() << "Json says: " << j.get().dump() << std::endl;
            } else {
                kzo.client.dbg() << "Response body: "
                          << std::get<BaseJob::BytesBody>(r.body) << std::endl;
            }
            return { std::move(m), lager::noop };
        }

        kzo.client.dbg() << "Sync successful" << std::endl;

        auto rooms = r.rooms();
        auto accountData = r.accountData();
        auto presence = r.presence();
        // load the info that has been sync'd

        m.syncToken = r.nextBatch();

        m.addTrigger(SyncSuccessful{r.nextBatch()});

        if (rooms) {
            m.addTriggers(loadRoomsFromSyncInPlace(m, std::move(rooms.value())));
        }

        if (presence) {
            m.addTriggers(loadPresenceFromSyncInPlace(m, std::move(presence.value().events)));
        }

        if (accountData) {
            m.addTriggers(loadAccountDataFromSyncInPlace(m, std::move(accountData.value().events)));
        }

        // TODO: process toDevice, deviceLists, deviceOneTimeKeysCount

        return { std::move(m), lager::noop };
    }

    ClientResult updateClient(ClientModel m, PostInitialFiltersAction)
    {
        if (m.syncing) {
            return { std::move(m), lager::noop };
        }

        Filter initialSyncFilter;
        initialSyncFilter.room.timeline.limit = 1;
        initialSyncFilter.room.state.lazyLoadMembers = true;
        auto firstJob = m.job<DefineFilterJob>()
            .make(m.userId, initialSyncFilter)
            .withData(json{{"is", "initialSyncFilter"}})
            .withQueue("post-filter", CancelFutureIfFailed);
        kzo.client.dbg() << "First filter: " << firstJob.requestBody() << std::endl;
        m.addJob(firstJob);
        Filter incrementalSyncFilter;
        incrementalSyncFilter.room.timeline.limit = 20;
        incrementalSyncFilter.room.state.lazyLoadMembers = true;
        m.addJob(m.job<DefineFilterJob>()
                 .make(m.userId, incrementalSyncFilter)
                 .withData(json{{"is", "incrementalSyncFilter"}})
                 .withQueue("post-filter", CancelFutureIfFailed));

        m.syncing = true;

        return { std::move(m), lager::noop };
    }

    ClientResult processResponse(ClientModel m, DefineFilterResponse r)
    {
        auto is = r.dataStr("is");
        if (! r.success()) {
            m.syncing = false;
            kzo.client.dbg() << "posting filter failed: " << r.errorCode() << r.errorMessage() << std::endl;
            m.addTrigger(PostInitialFiltersFailed{r.errorCode(), r.errorMessage()});
            return { std::move(m), lager::noop };
        }

        kzo.client.dbg() << "filter " << is << " is posted" << std::endl;

        if (is == "incrementalSyncFilter") {
            m.incrementalSyncFilterId = r.filterId();
            m.addTrigger(PostInitialFiltersSuccessful{});
        } else {
            m.initialSyncFilterId = r.filterId();
        }

        return { std::move(m), lager::noop };
    }
}