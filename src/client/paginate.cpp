
#include <lager/util.hpp>

#include "types.hpp"
#include "debug.hpp"
#include "client.hpp"
#include "csapi/message_pagination.hpp"

namespace Kazv
{
    Client::Effect paginateTimelineEffect(Client m, Client::PaginateTimelineAction a)
    {
        return
            [=](auto &&ctx) {
                auto roomId = a.roomId;
                auto room = m.roomList.at(roomId);
                auto paginateBackToken = room.paginateBackToken;

                auto job = m.job<GetRoomEventsJob>().make(
                    roomId,
                    paginateBackToken, // from
                    "b"s, // dir
                    defVal, // to
                    a.limit);

                auto &jobHandler = lager::get<JobInterface &>(ctx);
                jobHandler.fetch(
                    job,
                    [=](BaseJob::Response r) {
                        if (! GetRoomEventsJob::success(r)) {
                            dbgClient << "Get room events failed" << std::endl;
                            return;
                        }
                        auto paginateBackToken = GetRoomEventsJob::end(r);
                        auto events = GetRoomEventsJob::chunk(r);

                        Client::LoadPaginateTimelineResultAction
                            action{roomId, events, paginateBackToken};
                        ctx.dispatch(action);
                    });
            };
    }
}
