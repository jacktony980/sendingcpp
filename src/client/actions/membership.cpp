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

#include <csapi/create_room.hpp>
#include <csapi/inviting.hpp>
#include <csapi/joining.hpp>
#include <debug.hpp>

#include "client.hpp"
#include "clientutil.hpp"
#include "cursorutil.hpp"

#include "membership.hpp"

namespace Kazv
{
    static std::string visibilityToStr(CreateRoomAction::Visibility v)
    {
        using V = CreateRoomAction::Visibility;
        switch (v) {
        case V::Private:
            return "private";
        case V::Public:
            return "public";
        default:
            // should not happen
            return "";
        }
    }

    static std::string presetToStr(CreateRoomAction::Preset p)
    {
        using P = CreateRoomAction::Preset;
        switch (p) {
        case P::PrivateChat:
            return "private_chat";
        case P::PublicChat:
            return "public_chat";
        case P::TrustedPrivateChat:
            return "trusted_private_chat";
        default:
            // should not happen
            return "";
        }
    }

    ClientResult updateClient(Client m, CreateRoomAction a)
    {
        return {
            m,
            [=](auto &&ctx) {
                auto visibility = visibilityToStr(a.visibility);
                auto preset = a.preset
                    ? presetToStr(a.preset.value())
                    : ""s;

                using StateEvT = Kazv::CreateRoomJob::StateEvent;
                auto initialState = intoImmer(
                    immer::array<StateEvT>{},
                    zug::map(
                        [](Event e) {
                            return StateEvT{e.type(), e.stateKey(), e.content()};
                        }),
                    a.initialState);

                auto job = m.job<CreateRoomJob>().make(
                    visibility,
                    a.roomAliasName,
                    a.name,
                    a.topic,
                    a.invite,
                    DEFVAL, // invite3pid, not supported yet
                    a.roomVersion,
                    a.creationContent,
                    initialState,
                    preset,
                    a.isDirect,
                    a.powerLevelContentOverride);

                auto &jobHandler = lager::get<JobInterface &>(ctx);

                jobHandler.fetch(
                    job,
                    [=](BaseJob::Response r) {
                        if (!CreateRoomJob::success(r)) {
                            dbgClient << "Create room failed" << std::endl;
                            if (BaseJob::isBodyJson(r.body)) {
                                dbgClient << jsonBody(r).get().dump() << std::endl;
                                ctx.dispatch(Error::SetErrorAction{
                                        Error::JsonError{json{
                                                {"action", "CreateRoomAction"},
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

    ClientResult updateClient(Client m, InviteToRoomAction a)
    {
        return {
            m,
            [=](auto &&ctx) {
                auto job = m.job<InviteUserJob>().make(a.roomId, a.userId);

                auto &jobHandler = getJobHandler(ctx);
                jobHandler.fetch(
                    job,
                    [=](BaseJob::Response r) {
                        if (! InviteUserJob::success(r)) {
                            // Error
                            dbgClient << "Error inviting user" << std::endl;
                            return;
                        }
                        dbgClient << "Inviting user successful" << std::endl;
                    });
            }
        };
    }

    ClientResult updateClient(Client m, JoinRoomAction a)
    {
        return {
            m,
            [=](auto &&ctx) {
                auto job = m.job<JoinRoomJob>().make(a.roomIdOrAlias, a.serverName);

                auto &jobHandler = getJobHandler(ctx);
                jobHandler.fetch(
                    job,
                    [=](BaseJob::Response r) {
                        if (! JoinRoomJob::success(r)) {
                            // Error
                            auto &eventEmitter = getEventEmitter(ctx);
                            eventEmitter.emit(FailedToJoinRoom{
                                    a.roomIdOrAlias,
                                    r.errorCode(),
                                    r.errorMessage()
                                });
                            dbgClient << "Error joining room" << std::endl;
                            return;
                        }
                        dbgClient << "Successfully joined room" << std::endl;
                    });
            }
        };
    }

    ClientResult updateClient(Client m, JoinRoomByIdAction a)
    {
        return {
            m,
            [=](auto &&ctx) {
                auto job = m.job<JoinRoomByIdJob>().make(a.roomId);

                auto &jobHandler = getJobHandler(ctx);
                jobHandler.fetch(
                    job,
                    [=](BaseJob::Response r) {
                        if (! JoinRoomByIdJob::success(r)) {
                            // Error
                            dbgClient << "Error joining room" << std::endl;
                            return;
                        }
                        dbgClient << "Successfully joined room" << std::endl;
                    });
            }
        };
    }
};
