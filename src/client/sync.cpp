
#include <lager/util.hpp>

#include "client.hpp"
#include "csapi/sync.hpp"
#include "job/jobinterface.hpp"
#include "debug.hpp"

namespace Kazv
{
    Client::Effect syncEffect(Client m, Client::SyncAction a)
    {
        return
            [=](auto &&ctx) {
                dbgClient << "Server: " << m.serverUrl << std::endl
                          << "User: " << m.userId << std::endl
                          << "Access token: " << m.token << std::endl;
                SyncJob job(m.serverUrl,
                            m.token,
                            {}, // filter
                            a.since);
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
                        dbgClient << "Next batch token:" << SyncJob::nextBatch(r) << std::endl;
                        //dbgClient << "Json: " << jsonBody(r).get().dump() << std::endl;
                        auto accountData = SyncJob::accountData(r);
                        if (accountData) {
                            dbgClient << "Events in account data: " << std::endl;
                            auto events = accountData.value().events;
                            for (auto e : events) {
                                dbgClient << e.get().dump() << std::endl;
                            }
                        }
                    });
            };
    }
}
