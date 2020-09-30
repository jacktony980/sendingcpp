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


#include "csapi/rooms.hpp"
#include "csapi/room_state.hpp"

#include "util.hpp"
#include "client/cursorutil.hpp"
#include "debug.hpp"

#include "states.hpp"

namespace Kazv
{
    ClientResult updateClient(Client m, GetRoomStatesAction a)
    {
        auto eff =
            [=, roomId=a.roomId](auto &&ctx) {
                auto job = m.job<GetRoomStateJob>().make(roomId);
                auto &jobHandler = getJobHandler(ctx);

                jobHandler.fetch(
                    job,
                    [=](BaseJob::Response r) {
                        if (GetRoomStateJob::success(r)) {
                            auto events = GetRoomStateJob::data(r);
                            ctx.dispatch(LoadRoomStatesAction{roomId, events});
                        }
                    });
            };
        return {std::move(m), std::move(eff)};
    }

    ClientResult updateClient(Client m, LoadRoomStatesAction a)
    {
        auto action = RoomList::UpdateRoomAction{
            std::move(a.roomId),
            Room::AddStateEventsAction{std::move(a.events)}
        };
        m.roomList = RoomList::update(std::move(m.roomList), action);
        return {std::move(m), lager::noop};
    }

    ClientResult updateClient(Client m, SendStateEventAction a)
    {
        return {
            m,
            [=](auto &&ctx) {
                auto event = a.event;

                if (event.type() == ""s) {
                    ctx.dispatch(Error::SetErrorAction{
                            Error::JsonError{json{
                                    {"action", "SendStateEventAction"},
                                    {"reason", "Event has no type"}
                                }}
                        });
                }

                auto type = event.type();
                auto content = event.content();
                auto stateKey = event.stateKey();

                dbgClient << "Sending state event of type " << type
                          << " with content " << content.get().dump()
                          << " to " << a.roomId
                          << " with state key #" << stateKey << std::endl;

                auto job = m.job<SetRoomStateWithKeyJob>().make(
                    a.roomId,
                    type,
                    stateKey,
                    content);

                auto &jobHandler = lager::get<JobInterface &>(ctx);

                jobHandler.fetch(
                    job,
                    [=](BaseJob::Response r) {
                        if (!SetRoomStateWithKeyJob::success(r)) {
                            dbgClient << "Send state event failed" << std::endl;
                            if (BaseJob::isBodyJson(r.body)) {
                                dbgClient << jsonBody(r).get().dump() << std::endl;
                                ctx.dispatch(Error::SetErrorAction{
                                        Error::JsonError{json{
                                                {"action", "SendMessageAction"},
                                                {"reason", "Request to remote host failed"},
                                                {"original", jsonBody(r).get()}
                                            }}
                                    });
                            } else {
                                dbgClient << std::get<BaseJob::BytesBody>(r.body) << std::endl;
                            }
                        }
                        // if success, nothing to do
                    });
            }
        };
    }
}
