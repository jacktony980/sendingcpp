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

#include <libkazv-config.hpp>


#include "constants.hpp"
#include "login.hpp"

namespace Kazv
{
    GetLoginFlowsJob::GetLoginFlowsJob(std::string serverUrl)
        : BaseJob(std::move(serverUrl), BASE_PATH + "/login"s, GET)
    {
    }


    json loginDataToJson(std::string serverUrl,
                         std::string username,
                         std::string password,
                         std::optional<std::string> deviceName)
    {
        json j = json::object();
        j["type"] = "m.login.password";
        j["identifier"] = json::object();
        j["identifier"]["type"] = "m.id.user";
        j["identifier"]["user"] = std::move(username);
        j["password"] = std::move(password);
        j["initial_device_display_name"] = deviceName
            ? std::move(deviceName.value())
            : "libkazv"s;

        return j;
    }


    LoginJob::LoginJob(std::string serverUrl,
                       std::string username,
                       std::string password,
                       std::optional<std::string> deviceName)
        : BaseJob(serverUrl,
                  BASE_PATH + "/login"s,
                  POST,
                  std::string{},
                  ReturnType::Json,
                  JsonBody(loginDataToJson(serverUrl,
                                           std::move(username),
                                           std::move(password),
                                           std::move(deviceName))))
    {
    }
};
