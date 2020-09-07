
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

#include "csapi/sync.hpp"
#include "job/jobinterface.hpp"
#include "eventemitter/eventinterface.hpp"
#include "error.hpp"
#include "data/room.hpp"

namespace Kazv
{
    struct Client
    {
        std::string serverUrl;
        std::string userId;
        std::string token;
        std::string deviceId;
        bool loggedIn;
        Error error;

        std::string syncToken;

        RoomList roomList;
        immer::map<std::string /* sender */, Event> presence;
        immer::map<std::string /* type */, Event> accountData;

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

        // actions:
        struct LoginAction {
            std::string serverUrl;
            std::string username;
            std::string password;
            std::optional<std::string> deviceName;
        };

        struct LoadUserInfoAction {
            std::string serverUrl;
            std::string userId;
            std::string token;
            std::string deviceId;
            bool loggedIn;
        };

        struct LogoutAction {};

        struct SyncAction {};

        struct LoadSyncResultAction
        {
            std::string syncToken;
            std::optional<SyncJob::Rooms> rooms;
            std::optional<EventBatch> presence;
            std::optional<EventBatch> accountData;
            JsonWrap toDevice;
            JsonWrap deviceLists;
            immer::map<std::string, int> deviceOneTimeKeysCount;
        };

        struct PaginateTimelineAction
        {
            std::string roomId;
            std::optional<int> limit;
        };

        struct LoadPaginateTimelineResultAction
        {
            std::string roomId;
            EventList events;
            std::string paginateBackToken;
        };

        using Action = std::variant<LoginAction,
                                    LogoutAction,
                                    LoadUserInfoAction,
                                    SyncAction,
                                    Error::Action,
                                    LoadSyncResultAction,
                                    PaginateTimelineAction,
                                    LoadPaginateTimelineResultAction,
                                    RoomList::Action
                                    >;
        using Effect = lager::effect<Action, lager::deps<JobInterface &, EventInterface &>>;
        using Result = std::pair<Client, Effect>;
        static Result update(Client m, Action a);
    };

    inline bool operator==(Client a, Client b)
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
            && a.accountData == b.accountData;
    }

    Client::Effect syncEffect(Client m, Client::SyncAction a);
    Client::Effect paginateTimelineEffect(Client m, Client::PaginateTimelineAction a);

#ifndef NDEBUG
    LAGER_CEREAL_STRUCT(Client::LoginAction);
    LAGER_CEREAL_STRUCT(Client::LoadUserInfoAction);
    LAGER_CEREAL_STRUCT(Client::LogoutAction);
    LAGER_CEREAL_STRUCT(Client::SyncAction);
    LAGER_CEREAL_STRUCT(Client::LoadSyncResultAction);
    LAGER_CEREAL_STRUCT(Client::PaginateTimelineAction);
    LAGER_CEREAL_STRUCT(Client::LoadPaginateTimelineResultAction);
#endif

    template<class Archive>
    void serialize(Archive &ar, Client &m, std::uint32_t const /*version*/)
    {
        ar(m.serverUrl, m.userId, m.token, m.deviceId, m.loggedIn,
           m.error,
           m.syncToken,
           m.roomList,
           m.presence,
           m.accountData);
    }
}
CEREAL_CLASS_VERSION(Kazv::Client, 0);
