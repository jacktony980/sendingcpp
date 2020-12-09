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
#include <string>
#include <optional>

#include <lager/context.hpp>
#include <boost/hana.hpp>

#ifndef NDEBUG
#include <lager/debug/cereal/struct.hpp>
#endif

#include <csapi/sync.hpp>
#include <jobinterface.hpp>
#include <eventinterface.hpp>

#include "clientfwd.hpp"
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

    struct ClientModel
    {
        std::string serverUrl;
        std::string userId;
        std::string token;
        std::string deviceId;
        bool loggedIn;
        Error error;

        std::string syncToken;

        RoomListModel roomList;
        immer::map<std::string /* sender */, Event> presence;
        immer::map<std::string /* type */, Event> accountData;

        std::string nextTxnId{DEFTXNID};
        immer::flex_vector<BaseJob> nextJobs;
        immer::flex_vector<KazvEvent> nextTriggers;

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

    struct ProcessResponseAction
    {
        Response response;
    };

    inline bool operator==(ClientModel a, ClientModel b)
    {
        return a.serverUrl == b.serverUrl
            && a.userId == b.userId
            && a.token == b.token
            && a.deviceId == b.deviceId
            && a.loggedIn == b.loggedIn
            && a.error == b.error
            && a.syncToken == b.syncToken
            && a.roomList == b.roomList
            && a.presence == b.presence
            && a.accountData == b.accountData
            && a.nextTxnId == b.nextTxnId
            && a.nextJobs == b.nextJobs
            && a.nextTriggers == b.nextTriggers;
    }

#ifndef NDEBUG
    LAGER_CEREAL_STRUCT(LoginAction);
    LAGER_CEREAL_STRUCT(TokenLoginAction);
    LAGER_CEREAL_STRUCT(LogoutAction);
    LAGER_CEREAL_STRUCT(SyncAction);
    LAGER_CEREAL_STRUCT(PaginateTimelineAction);
    LAGER_CEREAL_STRUCT(SendMessageAction);
    LAGER_CEREAL_STRUCT(SendStateEventAction);
    LAGER_CEREAL_STRUCT(CreateRoomAction);
    LAGER_CEREAL_STRUCT(GetRoomStatesAction);
    LAGER_CEREAL_STRUCT(InviteToRoomAction);
    LAGER_CEREAL_STRUCT(JoinRoomByIdAction);
    LAGER_CEREAL_STRUCT(JoinRoomAction);
    LAGER_CEREAL_STRUCT(LeaveRoomAction);
    LAGER_CEREAL_STRUCT(ForgetRoomAction);
    LAGER_CEREAL_STRUCT(SetTypingAction);
    LAGER_CEREAL_STRUCT(PostReceiptAction);
    LAGER_CEREAL_STRUCT(ProcessResponseAction);
    LAGER_CEREAL_STRUCT(SetReadMarkerAction);
#endif

    template<class Archive>
    void serialize(Archive &ar, ClientModel &m, std::uint32_t const /*version*/)
    {
        ar(m.serverUrl, m.userId, m.token, m.deviceId, m.loggedIn,
           m.error,
           m.syncToken,
           m.roomList,
           m.presence,
           m.accountData,
           m.nextTxnId);
    }
}
CEREAL_CLASS_VERSION(Kazv::ClientModel, 0);
