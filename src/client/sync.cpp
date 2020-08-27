
#include <lager/util.hpp>

#include "client.hpp"
#include "csapi/sync.hpp"
#include "job/jobinterface.hpp"
#include "debug.hpp"

static const int syncInterval = 2000; // ms

namespace Kazv
{
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
                    [=](std::shared_future<BaseJob::Response> res) {
                        auto r = res.get();
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

                        // replace sync token
                        ctx.dispatch(Client::LoadSyncTokenAction{SyncJob::nextBatch(r)});

                        // emit events
                        auto &eventEmitter = lager::get<EventInterface &>(ctx);
                        auto accountData = SyncJob::accountData(r);
                        if (accountData) {
                            auto events = accountData.value().events;
                            for (auto e : events) {
                                eventEmitter.emit(ReceivingAccountDataEvent{e});
                            }
                        }
                        auto presence = SyncJob::presence(r);
                        if (presence) {
                            for (auto e : presence.value().events) {
                                eventEmitter.emit(ReceivingPresenceEvent{e});
                            }
                        }
                        auto rooms = SyncJob::rooms(r);
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
