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

#include "client.hpp"
#include "csapi/sync.hpp"
#include "job/jobinterface.hpp"
#include "debug.hpp"

static const int syncInterval = 2000; // ms

namespace Kazv
{
    // Atomicity guaranteed: if the sync action is created
    // before an action that reasonably changes Client
    // (e.g. roll back to an earlier state, obtain other
    // events), but executed
    // after that action, the sync will still give continuous
    // data about the events. (Sync will not "skip" events)
    // This is because this function takes the sync token
    // from the Client model it is passed.
    Client::Effect syncEffect(Client m, Client::SyncAction)
    {
        return
            [=](auto &&ctx) {
                dbgClient << "Start syncing with token " << m.syncToken << std::endl;
                SyncJob job(m.serverUrl,
                            m.token,
                            {}, // filter
                            m.syncToken);
                auto &jobHandler = lager::get<JobInterface &>(ctx);
                jobHandler.fetch(
                    job,
                    [=](BaseJob::Response r) {
                        if (!SyncJob::success(r)) {
                            dbgClient << "Sync failed" << std::endl;
                            dbgClient << r.statusCode << std::endl;
                            if (BaseJob::isBodyJson(r.body)) {
                                auto j = jsonBody(r);
                                dbgClient << "Json says: " << j.get().dump() << std::endl;
                            } else {
                                dbgClient << "Response body: "
                                          << std::get<BaseJob::BytesBody>(r.body) << std::endl;
                            }
                            return;
                        }
                        dbgClient << "Sync successful" << std::endl;

                        auto rooms = SyncJob::rooms(r);
                        auto accountData = SyncJob::accountData(r);
                        auto presence = SyncJob::presence(r);
                        // load the info that has been sync'd

                        ctx.dispatch(
                            Client::LoadSyncResultAction{
                                SyncJob::nextBatch(r),
                                rooms,
                                presence,
                                accountData,
                                SyncJob::toDevice(r),
                                SyncJob::deviceLists(r),
                                SyncJob::deviceOneTimeKeysCount(r),
                            });

                        // emit events
                        auto &eventEmitter = lager::get<EventInterface &>(ctx);

                        if (accountData) {
                            auto events = accountData.value().events;
                            for (auto e : events) {
                                eventEmitter.emit(ReceivingAccountDataEvent{e});
                            }
                        }

                        if (presence) {
                            for (auto e : presence.value().events) {
                                eventEmitter.emit(ReceivingPresenceEvent{e});
                            }
                        }

                        if (rooms) {
                            for (auto [id, room] : rooms.value().join) {
                                for (auto e : room.timeline.events) {
                                    eventEmitter.emit(ReceivingRoomTimelineEvent{e, id});
                                }
                            }
                        }

                        // kick off next sync
                        auto &jobHandler = lager::get<JobInterface &>(ctx);
                        jobHandler.setTimeout(
                            [=]() {
                                ctx.dispatch(Client::SyncAction{});
                            }, syncInterval);
                    });
            };
    }
}
