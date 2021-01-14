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
#include <variant>
#include "types.hpp"
#include "event.hpp"
#include "basejob.hpp"

namespace Kazv
{
    struct LoginSuccessful {};

    struct LoginFailed
    {
        std::string errorCode;
        std::string error;
    };

    struct SyncSuccessful
    {
        std::string nextToken;
    };

    struct SyncFailed
    {
    };

    struct PostInitialFiltersSuccessful
    {
    };

    struct PostInitialFiltersFailed
    {
        std::string errorCode;
        std::string error;
    };

    struct ReceivingPresenceEvent { Event event; };

    struct ReceivingAccountDataEvent { Event event; };

    struct ReceivingRoomStateEvent {
        Event event;
        std::string roomId;
    };

    struct ReceivingRoomTimelineEvent {
        Event event;
        std::string roomId;
    };

    struct ReceivingRoomAccountDataEvent {
        Event event;
        std::string roomId;
    };

    struct ReceivingToDeviceMessage
    {
        Event event;
    };

    struct RoomMembershipChanged {
        RoomMembership membership;
        std::string roomId;
    };

    struct PaginateSuccessful
    {
        std::string roomId;
    };

    struct PaginateFailed
    {
        std::string roomId;
    };

    struct CreateRoomSuccessful
    {
        std::string roomId;
    };

    struct CreateRoomFailed
    {
        std::string errorCode;
        std::string error;
    };

    struct InviteUserSuccessful
    {
        std::string roomId;
        std::string userId;
    };

    struct InviteUserFailed
    {
        std::string roomId;
        std::string userId;
        std::string errorCode;
        std::string error;
    };

    struct JoinRoomSuccessful
    {
        std::string roomIdOrAlias;
    };

    struct JoinRoomFailed
    {
        std::string roomIdOrAlias;
        std::string errorCode;
        std::string error;
    };

    struct LeaveRoomSuccessful
    {
        std::string roomId;
    };

    struct LeaveRoomFailed
    {
        std::string roomId;
        std::string errorCode;
        std::string error;
    };

    struct ForgetRoomSuccessful
    {
        std::string roomId;
    };

    struct ForgetRoomFailed
    {
        std::string roomId;
        std::string errorCode;
        std::string error;
    };

    struct SendMessageSuccessful
    {
        std::string roomId;
        std::string eventId;
    };

    struct SendMessageFailed
    {
        std::string roomId;
        std::string errorCode;
        std::string error;
    };

    struct SendToDeviceMessageSuccessful
    {
        std::string userId;
        std::string deviceId;
        std::string txnId;
    };

    struct SendToDeviceMessageFailed
    {
        std::string userId;
        std::string deviceId;
        std::string txnId;
        std::string errorCode;
        std::string error;
    };

    struct InvalidMessageFormat
    {
    };

    struct GetRoomStatesSuccessful
    {
        std::string roomId;
    };

    struct GetRoomStatesFailed
    {
        std::string roomId;
        std::string errorCode;
        std::string error;
    };

    struct GetStateEventSuccessful
    {
        std::string roomId;
        JsonWrap content;
    };

    struct GetStateEventFailed
    {
        std::string roomId;
        std::string errorCode;
        std::string error;
    };

    struct SendStateEventSuccessful
    {
        std::string roomId;
        std::string eventId;
        std::string eventType;
        std::string stateKey;
    };

    struct SendStateEventFailed
    {
        std::string roomId;
        std::string eventType;
        std::string stateKey;
        std::string errorCode;
        std::string error;
    };

    struct SetTypingSuccessful
    {
        std::string roomId;
    };

    struct SetTypingFailed
    {
        std::string roomId;
        std::string errorCode;
        std::string error;
    };

    struct PostReceiptSuccessful
    {
        std::string roomId;
    };

    struct PostReceiptFailed
    {
        std::string roomId;
        std::string errorCode;
        std::string error;
    };

    struct SetReadMarkerSuccessful
    {
        std::string roomId;
    };

    struct SetReadMarkerFailed
    {
        std::string roomId;
        std::string errorCode;
        std::string error;
    };

    struct UploadContentSuccessful
    {
        std::string mxcUri;
        std::string uploadId;
    };

    struct UploadContentFailed
    {
        std::string uploadId;
        std::string errorCode;
        std::string error;
    };

    struct DownloadContentSuccessful
    {
        std::string mxcUri;
        immer::box<Bytes> content;
        std::optional<std::string> filename;
        std::optional<std::string> contentType;
    };

    struct DownloadContentFailed
    {
        std::string mxcUri;
        std::string errorCode;
        std::string error;
    };

    struct DownloadThumbnailSuccessful
    {
        std::string mxcUri;
        immer::box<Bytes> content;
        std::optional<std::string> contentType;
    };

    struct DownloadThumbnailFailed
    {
        std::string mxcUri;
        std::string errorCode;
        std::string error;
    };

    struct UnrecognizedResponse
    {
        Response response;
    };

    using KazvEvent = std::variant<
        // use this for placeholder of "no events yet"
        // otherwise the first LoginSuccessful event cannot be detected
        std::monostate,

        // matrix events
        ReceivingPresenceEvent,
        ReceivingAccountDataEvent,
        ReceivingRoomTimelineEvent,
        ReceivingRoomStateEvent,
        RoomMembershipChanged,
        ReceivingRoomAccountDataEvent,
        ReceivingToDeviceMessage,

        // auth
        LoginSuccessful, LoginFailed,
        // sync
        SyncSuccessful, SyncFailed,
        PostInitialFiltersSuccessful, PostInitialFiltersFailed,
        // paginate
        PaginateSuccessful, PaginateFailed,
        // membership
        CreateRoomSuccessful, CreateRoomFailed,
        InviteUserSuccessful, InviteUserFailed,
        JoinRoomSuccessful, JoinRoomFailed,
        LeaveRoomSuccessful, LeaveRoomFailed,
        ForgetRoomSuccessful, ForgetRoomFailed,
        // send
        SendMessageSuccessful, SendMessageFailed,
        SendToDeviceMessageSuccessful, SendToDeviceMessageFailed,
        InvalidMessageFormat,
        // states
        GetRoomStatesSuccessful, GetRoomStatesFailed,
        GetStateEventSuccessful, GetStateEventFailed,
        SendStateEventSuccessful, SendStateEventFailed,
        // ephemeral
        SetTypingSuccessful, SetTypingFailed,
        PostReceiptSuccessful, PostReceiptFailed,
        SetReadMarkerSuccessful, SetReadMarkerFailed,
        // content
        UploadContentSuccessful, UploadContentFailed,
        DownloadContentSuccessful, DownloadContentFailed,
        DownloadThumbnailSuccessful, DownloadThumbnailFailed,

        // general
        UnrecognizedResponse
        >;

    using KazvEventList = immer::flex_vector<KazvEvent>;
}
