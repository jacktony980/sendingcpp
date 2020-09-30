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


#include <lager/util.hpp>
#include <lager/context.hpp>
#include <functional>

#include <immer/flex_vector_transient.hpp>

#include "client.hpp"

#include "actions/states.hpp"
#include "actions/auth.hpp"
#include "actions/membership.hpp"
#include "actions/paginate.hpp"
#include "actions/send.hpp"
#include "actions/states.hpp"
#include "actions/sync.hpp"

namespace Kazv
{
    auto Client::update(Client m, Action a) -> Result
    {
        return lager::match(std::move(a))(
            [&](Error::Action a) -> Result {
                m.error = Error::update(m.error, a);
                return {std::move(m), lager::noop};
            },

            [&](RoomList::Action a) -> Result {
                m.roomList = RoomList::update(std::move(m.roomList), a);
                return {std::move(m), lager::noop};
            },
            [&](auto a) -> decltype(updateClient(m, a)) {
                return updateClient(m, a);
            }
            );
    }
}
