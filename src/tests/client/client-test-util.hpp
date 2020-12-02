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

#include <lager/store.hpp>
#include <lager/event_loop/manual.hpp>

#include <client/client-model.hpp>
#include <base/basejob.hpp>
using namespace Kazv;

ClientModel createTestClientModel();

Response createResponse(std::string jobId, JsonWrap j, JsonWrap data = {});

inline auto createTestClientStore()
{
    return lager::make_store<ClientAction>(
        createTestClientModel(),
        &ClientModel::update,
        lager::with_manual_event_loop{});
}
