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

#include <debug.hpp>
#include <jobinterface.hpp>
#include <eventinterface.hpp>
#include <csapi/login.hpp>

#include "auth.hpp"


namespace Kazv
{
    ClientResult updateClient(ClientModel m, LoginAction a)
    {
        return {
            m,
            [=](auto &&ctx) {
                LoginJob job(a.serverUrl,
                             "m.login.password"s, // type
                             UserIdentifier{ "m.id.user"s, json{{"user", a.username}} }, // identifier
                             a.password,
                             {}, // token, not used
                             {}, // device id, not used
                             a.deviceName.value_or("libkazv"));
                auto &jobHandler = lager::get<JobInterface &>(ctx);
                jobHandler.fetch(
                    job,
                    [=](BaseJob::Response r) {
                        if (LoginJob::success(r)) {
                            dbgClient << "Job success" << std::endl;
                            const json &j = jsonBody(r).get();
                            std::string serverUrl = j.contains("well_known")
                                ? j.at("well_known").at("m.homeserver").at("base_url").get<std::string>()
                                : a.serverUrl;
                            ctx.dispatch(LoadUserInfoAction{
                                    serverUrl,
                                    j.at("user_id"),
                                    j.at("access_token"),
                                    j.at("device_id"),
                                    /* loggedIn = */ true
                                });
                        }
                    });
            }
        };
    }

    ClientResult updateClient(ClientModel m, LogoutAction)
    {
        return {
            m,
            [=](auto &&ctx) {
                ctx.dispatch(LoadUserInfoAction{
                        ""s,
                        ""s,
                        ""s,
                        ""s,
                        /* loggedIn = */ true
                    });
            }
        };
    }

    ClientResult updateClient(ClientModel m, LoadUserInfoAction a)
    {
        dbgClient << "LoadUserInfoAction: " << a.userId << std::endl;

        m.serverUrl = a.serverUrl;
        m.userId = a.userId;
        m.token = a.token;
        m.deviceId = a.deviceId;
        m.loggedIn = a.loggedIn;

        return {
            std::move(m),
            [](auto &&ctx) {
                auto &eventEmitter = lager::get<EventInterface &>(ctx);
                eventEmitter.emit(LoginSuccessful{});
                ctx.dispatch(Error::SetErrorAction{Error::NoError{}});
                // after user info is loaded, do first sync
                ctx.dispatch(SyncAction{});
            }
        };
    }

}
