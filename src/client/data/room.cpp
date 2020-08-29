
#include <lager/util.hpp>

#include "room.hpp"


namespace Kazv
{
    Room Room::update(Room r, Action a)
    {
        return lager::match(std::move(a))(
            [=](AddStateEventsAction a) mutable {
                r.stateEvents = r.stateEvents + a.stateEvents;
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
                r.accountData = r.accountData + a.events;
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
            },
            [=](LoadRoomsFromSyncAction a) mutable {
                auto updateRoomImpl =
                    [&l](auto id, auto a) {
                        l = RoomList::update(
                            std::move(l),
                            RoomList::UpdateRoomAction{id, a});
                    };
                auto updateSingleRoom =
                    [updateRoomImpl](auto id, auto room, auto membership) {
                        updateRoomImpl(id, Room::ChangeMembershipAction{membership});
                        updateRoomImpl(id, Room::AppendTimelineAction{room.timeline.events});
                        if (room.state) {
                            updateRoomImpl(id, Room::AddStateEventsAction{room.state.value().events});
                        }
                        if (room.accountData) {
                            updateRoomImpl(id, Room::AddAccountDataAction{room.accountData.value().events});
                        }
                    };

                auto updateJoinedRoom =
                    [=](auto id, auto room) {
                        updateSingleRoom(id, room, Room::Membership::Join);
                    };

                auto updateLeftRoom =
                    [=](auto id, auto room) {
                        updateSingleRoom(id, room, Room::Membership::Leave);
                    };

                for (auto &&[id, room]: a.rooms.join) {
                    updateJoinedRoom(id, room);
                }

                for (auto &&[id, room]: a.rooms.leave) {
                    updateLeftRoom(id, room);
                }

                return l;
            }
            );
    }
}
