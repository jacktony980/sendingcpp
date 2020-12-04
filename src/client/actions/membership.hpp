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

#include "client-model.hpp"
#include <csapi/create_room.hpp>
#include <csapi/inviting.hpp>
#include <csapi/joining.hpp>
#include <csapi/leaving.hpp>

namespace Kazv
{
    ClientResult updateClient(ClientModel m, CreateRoomAction a);
    ClientResult processResponse(ClientModel m, CreateRoomResponse r);

    ClientResult updateClient(ClientModel m, InviteToRoomAction a);
    ClientResult processResponse(ClientModel m, InviteUserResponse r);

    ClientResult updateClient(ClientModel m, JoinRoomByIdAction a);
    ClientResult processResponse(ClientModel m, JoinRoomByIdResponse r);

    ClientResult updateClient(ClientModel m, JoinRoomAction a);
    ClientResult processResponse(ClientModel m, JoinRoomResponse r);

    ClientResult updateClient(ClientModel m, LeaveRoomAction a);
    ClientResult processResponse(ClientModel m, LeaveRoomResponse r);

    ClientResult updateClient(ClientModel m, ForgetRoomAction a);
    ClientResult processResponse(ClientModel m, ForgetRoomResponse r);
}
