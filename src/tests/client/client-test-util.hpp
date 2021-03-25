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

#pragma once
#include <libkazv-config.hpp>

#include <lager/store.hpp>
#include <lager/event_loop/manual.hpp>

#include <store.hpp>

#include <client/client-model.hpp>
#include <base/basejob.hpp>
using namespace Kazv;

ClientModel createTestClientModel();

template<class DataT>
inline Response createResponse(std::string jobId, DataT body, JsonWrap data = {})
{
    Response r;
    r.statusCode = 200;
    r.body = body;
    json origData = data.get();
    origData["-job-id"] = jobId;
    r.extraData = origData;
    return r;
}

template<>
inline Response createResponse(std::string jobId, json j, JsonWrap data)
{
    return createResponse(jobId, JsonWrap(j), data);
}

inline auto createTestClientStore(SingleTypePromiseInterface<DefaultRetType> ph)
{
    return makeStore<ClientAction>(
        createTestClientModel(),
        &ClientModel::update,
        std::move(ph));
}

inline auto createTestClientStoreFrom(ClientModel m, SingleTypePromiseInterface<DefaultRetType> ph)
{
    return makeStore<ClientAction>(
        std::move(m),
        &ClientModel::update,
        std::move(ph));
}
