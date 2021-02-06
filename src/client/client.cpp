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

#include <lager/constant.hpp>

#include "client.hpp"

namespace Kazv
{
    Client::Client(lager::reader<SdkModel> sdk,
                   Context<ClientAction> ctx)
        : m_sdk(sdk)
        , m_client(sdk.map(&SdkModel::c))
        , m_ctx(std::move(ctx))
    {
    }

    Room Client::room(std::string id) const
    {
        return Room(m_sdk, lager::make_constant(id), m_ctx);
    }

    Room Client::roomByCursor(lager::reader<std::string> id) const
    {
        return Room(m_sdk, id, m_ctx);
    }

}
