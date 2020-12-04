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

#include "auth.hpp"


namespace Kazv
{
    ClientResult updateClient(ClientModel m, LoginAction a)
    {
        m.addJob(LoginJob{a.serverUrl,
                          "m.login.password"s, // type
                          UserIdentifier{ "m.id.user"s, json{{"user", a.username}} }, // identifier
                          a.password,
                          {}, // token, not used
                          {}, // device id, not used
                          a.deviceName.value_or("libkazv")}
            .withData(json{
                    {"serverUrl", a.serverUrl},
                }));
        return { m, lager::noop };
    }

    ClientResult processResponse(ClientModel m, LoginResponse r)
    {
        if (! r.success()) {
            m.addTrigger(LoginFailed{r.errorCode(), r.errorMessage()});
            return { std::move(m), lager::noop };
        }

        dbgClient << "Job success" << std::endl;
        const json &j = r.jsonBody().get();
        // TODO: replace this with r.wellKnown()
        std::string serverUrl = j.contains("well_known")
            ? j.at("well_known").at("m.homeserver").at("base_url").get<std::string>()
            : r.dataStr("serverUrl");

        m.serverUrl = serverUrl;
        m.userId = r.userId().value_or(DEFVAL);
        m.token = r.accessToken().value_or(DEFVAL);
        m.deviceId = r.deviceId().value_or(DEFVAL);
        m.loggedIn = true;

        m.addTrigger(LoginSuccessful{});

        return { std::move(m), lager::noop };
    }

    ClientResult updateClient(ClientModel m, TokenLoginAction a)
    {
        m.serverUrl = a.serverUrl;
        m.userId = a.username;
        m.token = a.token;
        m.deviceId = a.deviceId;
        m.loggedIn = true;

        m.addTrigger(LoginSuccessful{});
        return { std::move(m), lager::noop };
    }


    ClientResult updateClient(ClientModel m, LogoutAction)
    {
        // Note: this only performs a soft-logout.
        m.serverUrl = "";
        m.userId = "";
        m.token = "";
        m.deviceId = "";
        m.loggedIn = false;

        return { std::move(m), lager::noop };
    }

}
