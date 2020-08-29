
#pragma once

#include <tuple>
#include <variant>
#include <string>
#include <optional>

#include <lager/context.hpp>

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

        struct LoadSyncTokenAction {
            std::string syncToken;
        };

        using Action = std::variant<LoginAction,
                                    LogoutAction,
                                    LoadUserInfoAction,
                                    SyncAction,
                                    Error::Action,
                                    LoadSyncTokenAction,
                                    RoomList::Action
                                    >;
        using Effect = lager::effect<Action, lager::deps<JobInterface &, EventInterface &>>;
        using Result = std::pair<Client, Effect>;
        static Result update(Client m, Action a);
    };

    Client::Effect syncEffect(Client m, Client::SyncAction a);

#ifndef NDEBUG
    LAGER_CEREAL_STRUCT(Client::LoginAction);
    LAGER_CEREAL_STRUCT(Client::LoadUserInfoAction);
    LAGER_CEREAL_STRUCT(Client::LogoutAction);
    LAGER_CEREAL_STRUCT(Client::SyncAction);
    LAGER_CEREAL_STRUCT(Client::LoadSyncTokenAction);
    LAGER_CEREAL_STRUCT(Client, (serverUrl)(userId)(token)(deviceId)(loggedIn)(error)(syncToken)(roomList));
#endif
}
