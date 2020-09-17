/*
 * Copyright (C) 2020 Tusooa Zhu <tusooa@vista.aero>
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

#include "client.hpp"

#include "csapi/room_send.hpp"
#include "debug.hpp"

namespace Kazv
{
    Client::Effect sendMessageEffect(Client m, Client::SendMessageAction a)
    {
        return
            [=](auto &&ctx) {
                auto origJson = a.event.originalJson().get();
                if (!origJson.contains("type") || !origJson.contains("content")) {
                    ctx.dispatch(Error::SetErrorAction{
                            Error::JsonError{json{
                                    {"action", "SendMessageAction"},
                                    {"reason", "Event has no type or no content"}
                                }}
                        });
                }

                // We do not use event.type() etc. because we want
                // encrypted events stay encrypted.
                auto type = origJson["type"];
                auto content = origJson["content"];

                dbgClient << "Sending message of type " << type
                          << " with content " << content.dump()
                          << " to " << a.roomId
                          << " as #" << m.nextTxnId << std::endl;

                auto job = m.job<SendMessageJob>().make(
                    a.roomId,
                    type,
                    m.nextTxnId,
                    content);

                auto &jobHandler = lager::get<JobInterface &>(ctx);

                jobHandler.fetch(
                    job,
                    [=](BaseJob::Response r) {
                        if (!SendMessageJob::success(r)) {
                            dbgClient << "Send message failed" << std::endl;
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
            };
    }
};
