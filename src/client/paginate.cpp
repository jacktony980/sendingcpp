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
                    DEFVAL, // to
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
