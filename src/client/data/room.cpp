
#include <lager/util.hpp>

#include "room.hpp"


namespace Kazv
{
    Room Room::update(Room r, Action a)
    {
        return lager::match(std::move(a))(
            [=](AddStateEventsAction a) mutable {
                r.stateEvents = merge(std::move(r.stateEvents), a.stateEvents, keyOfState);
                return r;
            },
            [=](AppendTimelineAction a) mutable {
                r.timeline = r.timeline + a.events;
                return r;
            },
            [=](PrependTimelineAction a) mutable {
                r.timeline = a.events + r.timeline;
                return r;
            },
            [=](AddAccountDataAction a) mutable {
                r.accountData = merge(std::move(r.accountData), a.events, keyOfAccountData);
                return r;
            },
            [=](ChangeMembershipAction a) mutable {
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
