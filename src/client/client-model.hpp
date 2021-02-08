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
#include <libkazv-config.hpp>

#include <tuple>
#include <variant>
#include <string>
#include <optional>

#include <lager/context.hpp>
#include <boost/hana.hpp>

#ifndef NDEBUG
#include <lager/debug/cereal/struct.hpp>
#endif

#include <csapi/sync.hpp>

#include <crypto.hpp>

#include "clientfwd.hpp"
#include "device-list-tracker.hpp"
#include "error.hpp"
#include "room/room-model.hpp"

namespace Kazv
{
    inline const std::string DEFTXNID{"0"};

    enum RoomVisibility
    {
        Private,
        Public,
    };

    enum CreateRoomPreset
    {
        PrivateChat,
        PublicChat,
        TrustedPrivateChat,
    };

    enum ThumbnailResizingMethod
    {
        Crop,
        Scale,
    };

    struct ClientModel
    {
        std::string serverUrl;
        std::string userId;
        std::string token;
        std::string deviceId;
        bool loggedIn{false};
        Error error;

        bool syncing{false};
        int syncTimeoutMs{20000};
        std::string initialSyncFilterId;
        std::string incrementalSyncFilterId;
        std::optional<std::string> syncToken;

        RoomListModel roomList;
        immer::map<std::string /* sender */, Event> presence;
        immer::map<std::string /* type */, Event> accountData;

        std::string nextTxnId{DEFTXNID};
        immer::flex_vector<BaseJob> nextJobs;
        immer::flex_vector<KazvEvent> nextTriggers;

        EventList toDevice;
        std::optional<Crypto> crypto;
        bool identityKeysUploaded{false};

        DeviceListTracker deviceLists;

        immer::flex_vector<std::string /* deviceId */> devicesToSendKeys(std::string userId) const;

        std::pair<Event, std::optional<std::string> /* sessionKey */> megOlmEncrypt(Event e, std::string roomId);
        /// precondition: the one-time keys for those devices must already be claimed
        Event olmEncrypt(Event e, immer::map<std::string, immer::flex_vector<std::string>> userIdToDeviceIdMap);

        // helpers
        template<class Job>
        struct MakeJobT
        {
            template<class ...Args>
            constexpr auto make(Args &&...args) const {
                if constexpr (Job::needsAuth()) {
                    return Job(
                        serverUrl,
                        token,
                        std::forward<Args>(args)...);
                } else {
                    return Job(
                        serverUrl,
                        std::forward<Args>(args)...);
                }
            }

            std::string serverUrl;
            std::string token;
        };

        template<class Job>
        constexpr auto job() const {
            return MakeJobT<Job>{serverUrl, token};
        }

        inline void addJob(BaseJob j) {
            nextJobs = std::move(nextJobs).push_back(std::move(j));
        }

        inline auto popAllJobs() {
            auto jobs = std::move(nextJobs);
            nextJobs = DEFVAL;
            return jobs;
        };

        inline void addTrigger(KazvEvent t) {
            addTriggers({t});
        }

        inline void addTriggers(immer::flex_vector<KazvEvent> c) {
            nextTriggers = std::move(nextTriggers) + c;
        }

        inline auto popAllTriggers() {
            auto triggers = std::move(nextTriggers);
            nextTriggers = DEFVAL;
            return triggers;
        }

        using Action = ClientAction;
        using Effect = ClientEffect;
        using Result = ClientResult;

        static Result update(ClientModel m, Action a);
    };

    // actions:
    struct LoginAction {
        std::string serverUrl;
        std::string username;
        std::string password;
        std::optional<std::string> deviceName;
    };

    struct TokenLoginAction
    {
        std::string serverUrl;
        std::string username;
        std::string token;
        std::string deviceId;
    };

    struct LogoutAction {};

    struct SyncAction {};

    struct PaginateTimelineAction
    {
        std::string roomId;
        std::optional<int> limit;
    };

    struct SendMessageAction
    {
        std::string roomId;
        Event event;
    };

    struct SendStateEventAction
    {
        std::string roomId;
        Event event;
    };

    struct CreateRoomAction
    {
        using Visibility = RoomVisibility;
        using Preset = CreateRoomPreset;
        Visibility visibility;
        std::optional<std::string> roomAliasName;
        std::optional<std::string> name;
        std::optional<std::string> topic;
        immer::array<std::string> invite;
        //immer::array<Invite3pid> invite3pid;
        std::optional<std::string> roomVersion;
        JsonWrap creationContent;
        immer::array<Event> initialState;
        std::optional<Preset> preset;
        std::optional<bool> isDirect;
        JsonWrap powerLevelContentOverride;
    };

    struct GetRoomStatesAction
    {
        std::string roomId;
    };

    struct GetStateEventAction
    {
        std::string roomId;
        std::string type;
        std::string stateKey;
    };

    struct InviteToRoomAction
    {
        std::string roomId;
        std::string userId;
    };

    struct JoinRoomByIdAction
    {
        std::string roomId;
    };

    struct JoinRoomAction
    {
        std::string roomIdOrAlias;
        immer::array<std::string> serverName;
    };

    struct LeaveRoomAction
    {
        std::string roomId;
    };

    struct ForgetRoomAction
    {
        std::string roomId;
    };


    struct SetTypingAction
    {
        std::string roomId;
        bool typing;
        std::optional<int> timeoutMs;
    };

    struct PostReceiptAction
    {
        std::string roomId;
        std::string eventId;
    };

    struct SetReadMarkerAction
    {
        std::string roomId;
        std::string eventId;
    };

    struct UploadContentAction
    {
        immer::box<Bytes> content;
        std::optional<std::string> filename;
        std::optional<std::string> contentType;
        std::string uploadId; // to be used by library users
    };

    struct DownloadContentAction
    {
        std::string mxcUri;
    };

    struct DownloadThumbnailAction
    {
        std::string mxcUri;
        int width;
        int height;
        std::optional<ThumbnailResizingMethod> method;
        std::optional<bool> allowRemote;
    };

    struct ResubmitJobAction
    {
        BaseJob job;
    };

    struct ProcessResponseAction
    {
        Response response;
    };

    struct PostInitialFiltersAction
    {
    };

    struct SendToDeviceMessageAction
    {
        Event event;
        immer::map<std::string, immer::flex_vector<std::string>> devicesToSend;
    };

    struct UploadIdentityKeysAction
    {
    };

    struct GenerateAndUploadOneTimeKeysAction
    {
    };

    struct QueryKeysAction
    {
        bool isInitialSync;
    };

    struct ClaimKeysAndSendSessionKeyAction
    {
        std::string roomId;
        std::string sessionId;
        std::string sessionKey;
        immer::map<std::string, immer::flex_vector<std::string>> devicesToSend;
    };

#ifndef NDEBUG
    LAGER_CEREAL_STRUCT(LoginAction);
    LAGER_CEREAL_STRUCT(TokenLoginAction);
    LAGER_CEREAL_STRUCT(LogoutAction);
    LAGER_CEREAL_STRUCT(SyncAction);
    LAGER_CEREAL_STRUCT(PostInitialFiltersAction);

    LAGER_CEREAL_STRUCT(PaginateTimelineAction);
    LAGER_CEREAL_STRUCT(SendMessageAction);
    LAGER_CEREAL_STRUCT(SendStateEventAction);
    LAGER_CEREAL_STRUCT(SendToDeviceMessageAction);
    LAGER_CEREAL_STRUCT(CreateRoomAction);
    LAGER_CEREAL_STRUCT(GetRoomStatesAction);
    LAGER_CEREAL_STRUCT(GetStateEventAction);
    LAGER_CEREAL_STRUCT(InviteToRoomAction);
    LAGER_CEREAL_STRUCT(JoinRoomByIdAction);
    LAGER_CEREAL_STRUCT(JoinRoomAction);
    LAGER_CEREAL_STRUCT(LeaveRoomAction);
    LAGER_CEREAL_STRUCT(ForgetRoomAction);
    LAGER_CEREAL_STRUCT(SetTypingAction);
    LAGER_CEREAL_STRUCT(PostReceiptAction);
    LAGER_CEREAL_STRUCT(ProcessResponseAction);
    LAGER_CEREAL_STRUCT(SetReadMarkerAction);

    LAGER_CEREAL_STRUCT(UploadContentAction);
    LAGER_CEREAL_STRUCT(DownloadContentAction);
    LAGER_CEREAL_STRUCT(DownloadThumbnailAction);

    LAGER_CEREAL_STRUCT(ResubmitJobAction);
#endif

    template<class Archive>
    void serialize(Archive &ar, ClientModel &m, std::uint32_t const /*version*/)
    {
        ar(m.serverUrl, m.userId, m.token, m.deviceId, m.loggedIn,
           m.error,
           m.initialSyncFilterId,
           m.incrementalSyncFilterId,
           m.syncToken,
           m.roomList,
           m.presence,
           m.accountData,
           m.nextTxnId,
           m.toDevice);
    }
}
CEREAL_CLASS_VERSION(Kazv::ClientModel, 0);
