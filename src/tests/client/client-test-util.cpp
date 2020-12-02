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

#include "client-test-util.hpp"

ClientModel createTestClientModel()
{
    ClientModel m;
    m.serverUrl = "example.com";
    m.userId = "@bob:example.com";
    m.token = "exampletoken";
    m.deviceId = "exampledevice";
    m.loggedIn = true;
    return m;
}

Response createResponse(std::string jobId, JsonWrap j, JsonWrap data)
{
    Response r;
    r.statusCode = 200;
    r.body = j;
    json origData = data.get();
    origData["-job-id"] = jobId;
    r.extraData = origData;
    return r;
}
