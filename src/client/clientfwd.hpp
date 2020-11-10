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

#pragma once

#include <tuple>
#include <variant>

#include <lager/context.hpp>

#include "error.hpp"
#include "room/room-model.hpp"

namespace Kazv
{
    class JobInterface;
    class EventInterface;

    struct LoginAction;
    struct LogoutAction;
    struct LoadUserInfoAction;
    struct SyncAction;
    struct LoadSyncResultAction;
    struct PaginateTimelineAction;
    struct LoadPaginateTimelineResultAction;
    struct SendMessageAction;
    struct SendStateEventAction;
    struct CreateRoomAction;
    struct GetRoomStatesAction;
    struct LoadRoomStatesAction;
    struct InviteToRoomAction;
    struct JoinRoomByIdAction;
    struct EmitKazvEventsAction;
    struct JoinRoomAction;

    struct ClientModel;

    using ClientAction = std::variant<
        LoginAction,
        LogoutAction,
        LoadUserInfoAction,
        SyncAction,
        Error::Action,
        LoadSyncResultAction,
        PaginateTimelineAction,
        LoadPaginateTimelineResultAction,
        SendMessageAction,
        SendStateEventAction,
        CreateRoomAction,
        GetRoomStatesAction,
        LoadRoomStatesAction,
        InviteToRoomAction,
        JoinRoomByIdAction,
        EmitKazvEventsAction,
        RoomListAction,
        JoinRoomAction
        >;

    using ClientEffect = lager::effect<ClientAction, lager::deps<JobInterface &, EventInterface &>>;

    using ClientResult = std::pair<ClientModel, ClientEffect>;
}
