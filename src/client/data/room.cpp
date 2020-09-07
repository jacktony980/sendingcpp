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

#include "room.hpp"


namespace Kazv
{
    Room Room::update(Room r, Action a)
    {
        return lager::match(std::move(a))(
            [&](AddStateEventsAction a) {
                r.stateEvents = merge(std::move(r.stateEvents), a.stateEvents, keyOfState);
                return r;
            },
            [&](AppendTimelineAction a) {
                r.timeline = r.timeline + a.events;
                return r;
            },
            [&](PrependTimelineAction a) {
                r.timeline = a.events + r.timeline;
                r.paginateBackToken = a.paginateBackToken;
                // if there are no more events we should not allow further paginating
                r.canPaginateBack = a.events.size() != 0;
                return r;
            },
            [&](AddAccountDataAction a) {
                r.accountData = merge(std::move(r.accountData), a.events, keyOfAccountData);
                return r;
            },
            [&](ChangeMembershipAction a) {
                r.membership = a.membership;
                return r;
            }
            );
    }

    RoomList RoomList::update(RoomList l, Action a)
    {
        return lager::match(std::move(a))(
            [=](UpdateRoomAction a) mutable {
                l.rooms = std::move(l.rooms)
                    .update(a.roomId,
                            [=](Room oldRoom) {
                                oldRoom.roomId = a.roomId; // in case it is a new room
                                return Room::update(std::move(oldRoom), a.roomAction);
                            });
                return l;
            }
            );
    }
}
