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

// eager requires tuplify but did not include it.
// This is a bug.
#include <zug/tuplify.hpp>
#include <zug/transducer/eager.hpp>
#include <zug/into.hpp>

#include <lager/util.hpp>

#include <types.hpp>
#include <debug.hpp>
#include <csapi/message_pagination.hpp>

#include <cursorutil.hpp>

#include "paginate.hpp"

namespace Kazv
{
    ClientResult updateClient(Client m, PaginateTimelineAction a)
    {
        return {
            m,
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

                        LoadPaginateTimelineResultAction
                            action{roomId, events, paginateBackToken};
                        ctx.dispatch(action);
                    });
            }
        };
    }

    ClientResult updateClient(Client m, LoadPaginateTimelineResultAction a)
    {
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
    }
}
