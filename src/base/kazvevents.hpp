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
    inline bool operator==(LoginSuccessful, LoginSuccessful)
    {
        return true;
    }

    struct LoginFailed
    {
        std::string errorCode;
        std::string error;
    };
    inline bool operator==(LoginFailed a, LoginFailed b)
    {
        return a.errorCode == b.errorCode
            && a.error == b.error;
    }

    struct SyncSuccessful
    {
        std::string nextToken;
    };

    inline bool operator==(SyncSuccessful a, SyncSuccessful b)
    {
        return a.nextToken == b.nextToken;
    }

    struct SyncFailed
    {
    };

    inline bool operator==(SyncFailed, SyncFailed)
    {
        return true;
    }

    struct PostInitialFiltersSuccessful
    {
    };

    inline bool operator==(PostInitialFiltersSuccessful, PostInitialFiltersSuccessful)
    {
        return true;
    }

    struct PostInitialFiltersFailed
    {
        std::string errorCode;
        std::string error;
    };

    inline bool operator==(PostInitialFiltersFailed a, PostInitialFiltersFailed b)
    {
        return a.errorCode == b.errorCode
            && a.error == b.error;
    }

    struct ReceivingPresenceEvent { Event event; };
    inline bool operator==(ReceivingPresenceEvent a, ReceivingPresenceEvent b)
    {
        return a.event == b.event;
    }

    struct ReceivingAccountDataEvent { Event event; };
    inline bool operator==(ReceivingAccountDataEvent a, ReceivingAccountDataEvent b)
    {
        return a.event == b.event;
    }

    struct ReceivingRoomStateEvent {
        Event event;
        std::string roomId;
    };

    inline bool operator==(ReceivingRoomStateEvent a, ReceivingRoomStateEvent b)
    {
        return a.event == b.event && a.roomId == b.roomId;
    }

    struct ReceivingRoomTimelineEvent {
        Event event;
        std::string roomId;
    };

    inline bool operator==(ReceivingRoomTimelineEvent a, ReceivingRoomTimelineEvent b)
    {
        return a.event == b.event && a.roomId == b.roomId;
    }

    struct ReceivingRoomAccountDataEvent {
        Event event;
        std::string roomId;
    };

    inline bool operator==(ReceivingRoomAccountDataEvent a, ReceivingRoomAccountDataEvent b)
    {
        return a.event == b.event && a.roomId == b.roomId;
    }

    struct RoomMembershipChanged {
        RoomMembership membership;
        std::string roomId;
    };

    inline bool operator==(RoomMembershipChanged a, RoomMembershipChanged b)
    {
        return a.membership == b.membership && a.roomId == b.roomId;
    }

    struct PaginateSuccessful
    {
        std::string roomId;
    };

    inline bool operator==(PaginateSuccessful a, PaginateSuccessful b)
    {
        return a.roomId == b.roomId;
    }

    struct PaginateFailed
    {
        std::string roomId;
    };

    inline bool operator==(PaginateFailed a, PaginateFailed b)
    {
        return a.roomId == b.roomId;
    }

    struct CreateRoomSuccessful
    {
        std::string roomId;
    };

    inline bool operator==(CreateRoomSuccessful a, CreateRoomSuccessful b)
    {
        return a.roomId == b.roomId;
    }

    struct CreateRoomFailed
    {
        std::string errorCode;
        std::string error;
    };

    inline bool operator==(CreateRoomFailed a, CreateRoomFailed b)
    {
        return a.errorCode == b.errorCode
            && a.error == b.error;
    }

    struct InviteUserSuccessful
    {
        std::string roomId;
        std::string userId;
    };

    inline bool operator==(InviteUserSuccessful a, InviteUserSuccessful b)
    {
        return a.roomId == b.roomId
            && a.userId == b.userId;
    }

    struct InviteUserFailed
    {
        std::string roomId;
        std::string userId;
        std::string errorCode;
        std::string error;
    };

    inline bool operator==(InviteUserFailed a, InviteUserFailed b)
    {
        return a.roomId == b.roomId
            && a.userId == b.userId
            && a.errorCode == b.errorCode
            && a.error == b.error;
    }

    struct JoinRoomSuccessful
    {
        std::string roomIdOrAlias;
    };

    inline bool operator==(JoinRoomSuccessful a, JoinRoomSuccessful b)
    {
        return a.roomIdOrAlias == b.roomIdOrAlias;
    };

    struct JoinRoomFailed
    {
        std::string roomIdOrAlias;
        std::string errorCode;
        std::string error;
    };

    inline bool operator==(JoinRoomFailed a, JoinRoomFailed b)
    {
        return a.roomIdOrAlias == b.roomIdOrAlias
            && a.errorCode == b.errorCode
            && a.error == b.error;
    };

    struct LeaveRoomSuccessful
    {
        std::string roomId;
    };

    inline bool operator==(LeaveRoomSuccessful a, LeaveRoomSuccessful b)
    {
        return a.roomId == b.roomId;
    };

    struct LeaveRoomFailed
    {
        std::string roomId;
        std::string errorCode;
        std::string error;
    };

    inline bool operator==(LeaveRoomFailed a, LeaveRoomFailed b)
    {
        return a.roomId == b.roomId
            && a.errorCode == b.errorCode
            && a.error == b.error;
    }

    struct ForgetRoomSuccessful
    {
        std::string roomId;
    };

    inline bool operator==(ForgetRoomSuccessful a, ForgetRoomSuccessful b)
    {
        return a.roomId == b.roomId;
    };

    struct ForgetRoomFailed
    {
        std::string roomId;
        std::string errorCode;
        std::string error;
    };

    inline bool operator==(ForgetRoomFailed a, ForgetRoomFailed b)
    {
        return a.roomId == b.roomId
            && a.errorCode == b.errorCode
            && a.error == b.error;
    }

    struct SendMessageSuccessful
    {
        std::string roomId;
        std::string eventId;
    };

    inline bool operator==(SendMessageSuccessful a, SendMessageSuccessful b)
    {
        return a.roomId == b.roomId
            && a.eventId == b.eventId;
    }

    struct SendMessageFailed
    {
        std::string roomId;
        std::string errorCode;
        std::string error;
    };

    inline bool operator==(SendMessageFailed a, SendMessageFailed b)
    {
        return a.roomId == b.roomId
            && a.errorCode == b.errorCode
            && a.error == b.error;
    }

    struct InvalidMessageFormat
    {
    };

    inline bool operator==(InvalidMessageFormat, InvalidMessageFormat)
    {
        return true;
    }

    struct GetRoomStatesSuccessful
    {
        std::string roomId;
    };

    inline bool operator==(GetRoomStatesSuccessful a, GetRoomStatesSuccessful b)
    {
        return a.roomId == b.roomId;
    }

    struct GetRoomStatesFailed
    {
        std::string roomId;
        std::string errorCode;
        std::string error;
    };

    inline bool operator==(GetRoomStatesFailed a, GetRoomStatesFailed b)
    {
        return a.roomId == b.roomId
            && a.errorCode == b.errorCode
            && a.error == b.error;
    }

    struct SendStateEventSuccessful
    {
        std::string roomId;
        std::string eventId;
        std::string eventType;
        std::string stateKey;
    };

    inline bool operator==(SendStateEventSuccessful a, SendStateEventSuccessful b)
    {
        return a.roomId == b.roomId
            && a.eventId == b.eventId
            && a.eventType == b.eventType
            && a.stateKey == b.stateKey;
    }

    struct SendStateEventFailed
    {
        std::string roomId;
        std::string eventType;
        std::string stateKey;
        std::string errorCode;
        std::string error;
    };

    inline bool operator==(SendStateEventFailed a, SendStateEventFailed b)
    {
        return a.roomId == b.roomId
            && a.eventType == b.eventType
            && a.stateKey == b.stateKey
            && a.errorCode == b.errorCode
            && a.error == b.error;
    }

    struct SetTypingSuccessful
    {
        std::string roomId;
    };

    inline bool operator==(SetTypingSuccessful a, SetTypingSuccessful b)
    {
        return a.roomId == b.roomId;
    }


    struct SetTypingFailed
    {
        std::string roomId;
        std::string errorCode;
        std::string error;
    };

    inline bool operator==(SetTypingFailed a, SetTypingFailed b)
    {
        return a.roomId == b.roomId
            && a.errorCode == b.errorCode
            && a.error == b.error;
    }

    struct PostReceiptSuccessful
    {
        std::string roomId;
    };

    inline bool operator==(PostReceiptSuccessful a, PostReceiptSuccessful b)
    {
        return a.roomId == b.roomId;
    }

    struct PostReceiptFailed
    {
        std::string roomId;
        std::string errorCode;
        std::string error;
    };

    inline bool operator==(PostReceiptFailed a, PostReceiptFailed b)
    {
        return a.roomId == b.roomId
            && a.errorCode == b.errorCode
            && a.error == b.error;
    }

    struct SetReadMarkerSuccessful
    {
        std::string roomId;
    };

    inline bool operator==(SetReadMarkerSuccessful a, SetReadMarkerSuccessful b)
    {
        return a.roomId == b.roomId;
    }

    struct SetReadMarkerFailed
    {
        std::string roomId;
        std::string errorCode;
        std::string error;
    };

    inline bool operator==(SetReadMarkerFailed a, SetReadMarkerFailed b)
    {
        return a.roomId == b.roomId
            && a.errorCode == b.errorCode
            && a.error == b.error;
    }

    struct UploadContentSuccessful
    {
        std::string mxcUri;
        std::string uploadId;
    };

    inline bool operator==(UploadContentSuccessful a, UploadContentSuccessful b)
    {
        return a.mxcUri == b.mxcUri
            && a.uploadId == b.uploadId;
    };

    struct UploadContentFailed
    {
        std::string uploadId;
        std::string errorCode;
        std::string error;
    };

    inline bool operator==(UploadContentFailed a, UploadContentFailed b)
    {
        return a.uploadId == b.uploadId
            && a.errorCode == b.errorCode
            && a.error == b.error;
    }

    struct DownloadContentSuccessful
    {
        std::string mxcUri;
        immer::box<Bytes> content;
        std::optional<std::string> filename;
        std::optional<std::string> contentType;
    };

    inline bool operator==(DownloadContentSuccessful a, DownloadContentSuccessful b)
    {
        return a.mxcUri == b.mxcUri
            && a.content == b.content
            && a.filename == b.filename
            && a.contentType == b.contentType;
    };

    struct DownloadContentFailed
    {
        std::string mxcUri;
        std::string errorCode;
        std::string error;
    };

    inline bool operator==(DownloadContentFailed a, DownloadContentFailed b)
    {
        return a.mxcUri == b.mxcUri
            && a.errorCode == b.errorCode
            && a.error == b.error;
    }

    struct DownloadThumbnailSuccessful
    {
        std::string mxcUri;
        immer::box<Bytes> content;
        std::optional<std::string> contentType;
    };

    inline bool operator==(DownloadThumbnailSuccessful a, DownloadThumbnailSuccessful b)
    {
        return a.mxcUri == b.mxcUri
            && a.content == b.content
            && a.contentType == b.contentType;
    };

    struct DownloadThumbnailFailed
    {
        std::string mxcUri;
        std::string errorCode;
        std::string error;
    };

    inline bool operator==(DownloadThumbnailFailed a, DownloadThumbnailFailed b)
    {
        return a.mxcUri == b.mxcUri
            && a.errorCode == b.errorCode
            && a.error == b.error;
    }

    struct UnrecognizedResponse
    {
        Response response;
    };

    inline bool operator==(UnrecognizedResponse a, UnrecognizedResponse b)
    {
        return a.response == b.response;
    }

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
        InvalidMessageFormat,
        // states
        GetRoomStatesSuccessful, GetRoomStatesFailed,
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

    inline bool operator!=(KazvEvent a, KazvEvent b)
    {
        return !(a == b);
    }

    using KazvEventList = immer::flex_vector<KazvEvent>;
}
