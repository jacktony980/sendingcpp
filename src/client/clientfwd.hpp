/*
 * Copyright (C) 2020-2021 Tusooa Zhu <tusooa@vista.aero>
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
#include <libkazv-config.hpp>

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
    struct TokenLoginAction;
    struct LogoutAction;
    struct SyncAction;
    struct PostInitialFiltersAction;
    struct PaginateTimelineAction;
    struct SendMessageAction;
    struct SendStateEventAction;
    struct CreateRoomAction;
    struct GetRoomStatesAction;
    struct GetStateEventAction;
    struct InviteToRoomAction;
    struct JoinRoomByIdAction;
    struct EmitKazvEventsAction;
    struct JoinRoomAction;
    struct LeaveRoomAction;
    struct ForgetRoomAction;

    struct ProcessResponseAction;
    struct SetTypingAction;
    struct PostReceiptAction;
    struct SetReadMarkerAction;

    struct UploadContentAction;
    struct DownloadContentAction;
    struct DownloadThumbnailAction;

    struct SendToDeviceMessageAction;

    struct UploadIdentityKeysAction;
    struct GenerateAndUploadOneTimeKeysAction;
    struct QueryKeysAction;
    struct ClaimKeysAndSendSessionKeyAction;

    struct ResubmitJobAction;

    struct ClientModel;

    using ClientAction = std::variant<
        RoomListAction,
        Error::Action,

        LoginAction,
        TokenLoginAction,
        LogoutAction,

        SyncAction,
        PostInitialFiltersAction,

        PaginateTimelineAction,
        SendMessageAction,
        SendStateEventAction,
        CreateRoomAction,
        GetRoomStatesAction,
        GetStateEventAction,
        InviteToRoomAction,
        JoinRoomByIdAction,

        JoinRoomAction,
        LeaveRoomAction,
        ForgetRoomAction,

        ProcessResponseAction,

        SetTypingAction,
        PostReceiptAction,
        SetReadMarkerAction,

        UploadContentAction,
        DownloadContentAction,
        DownloadThumbnailAction,

        SendToDeviceMessageAction,

        UploadIdentityKeysAction,
        GenerateAndUploadOneTimeKeysAction,
        QueryKeysAction,
        ClaimKeysAndSendSessionKeyAction,

        ResubmitJobAction
        >;

    using ClientEffect = lager::effect<ClientAction, lager::deps<>>;

    using ClientResult = std::pair<ClientModel, ClientEffect>;
}
