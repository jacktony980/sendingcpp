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
#include <zug/sequence.hpp>
#include <zug/transducer/map.hpp>

#include "room-model.hpp"
#include "cursorutil.hpp"


namespace Kazv
{
    RoomModel RoomModel::update(RoomModel r, Action a)
    {
        return lager::match(std::move(a))(
            [&](AddStateEventsAction a) {
                r.stateEvents = merge(std::move(r.stateEvents), a.stateEvents, keyOfState);
                return r;
            },
            [&](AppendTimelineAction a) {
                auto eventIds = intoImmer(immer::flex_vector<std::string>(),
                                          zug::map(keyOfTimeline), a.events);
                r.timeline = r.timeline + eventIds;
                r.messages = merge(std::move(r.messages), a.events, keyOfTimeline);
                return r;
            },
            [&](PrependTimelineAction a) {
                auto eventIds = intoImmer(immer::flex_vector<std::string>(),
                                          zug::map(keyOfTimeline), a.events);
                r.timeline = eventIds + r.timeline;
                r.messages = merge(std::move(r.messages), a.events, keyOfTimeline);
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
            },
            [&](ChangeInviteStateAction a) {
                r.inviteState = merge(immer::map<KeyOfState, Event>{}, a.events, keyOfState);
                return r;
            }
            );
    }

    RoomListModel RoomListModel::update(RoomListModel l, Action a)
    {
        return lager::match(std::move(a))(
            [=](UpdateRoomAction a) mutable {
                l.rooms = std::move(l.rooms)
                    .update(a.roomId,
                            [=](RoomModel oldRoom) {
                                oldRoom.roomId = a.roomId; // in case it is a new room
                                return RoomModel::update(std::move(oldRoom), a.roomAction);
                            });
                return l;
            }
            );
    }
}
