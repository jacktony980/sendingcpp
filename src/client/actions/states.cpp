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


#include <debug.hpp>

#include "clientutil.hpp"
#include "cursorutil.hpp"

#include "states.hpp"

namespace Kazv
{
    BaseJob clientPerform(ClientModel m, GetRoomStatesAction a)
    {
        auto roomId = a.roomId;
        return m.job<GetRoomStateJob>()
            .make(roomId)
            .withData(json{{"roomId", roomId}});
    }

    ClientResult updateClient(ClientModel m, GetRoomStatesAction a)
    {
        m.addJob(clientPerform(m, a));

        return { std::move(m), lager::noop };
    }

    ClientResult processResponse(ClientModel m, GetRoomStateResponse r)
    {
        auto roomId = r.dataStr("roomId");

        if (! r.success()) {
            m.addTrigger(GetRoomStatesFailed{roomId, r.errorCode(), r.errorMessage()});
            return { std::move(m), lager::noop };
        }

        m.addTrigger(GetRoomStatesSuccessful{roomId});

        auto events = r.data();

        auto action = UpdateRoomAction{
            roomId,
            AddStateEventsAction{std::move(events)}
        };
        m.roomList = RoomListModel::update(std::move(m.roomList), action);

        if (m.crypto && m.roomList[roomId].encrypted) {
            m.deviceLists.track(m.roomList[roomId].joinedMemberIds());
        }
        m.roomList = RoomListModel::update(std::move(m.roomList),
                                           UpdateRoomAction{roomId, MarkMembersFullyLoadedAction{}});

        //m.addTrigger(ShouldQueryKeys{false});

        return {std::move(m), lager::noop};
    }

    ClientResult updateClient(ClientModel m, GetStateEventAction a)
    {
        auto job = m.job<GetRoomStateWithKeyJob>()
            .make(a.roomId, a.type, a.stateKey)
            .withData(json{
                    {"roomId", a.roomId},
                    {"type", a.type},
                    {"stateKey", a.stateKey}
                });

        m.addJob(std::move(job));

        return { std::move(m), lager::noop };
    }

    ClientResult processResponse(ClientModel m, GetRoomStateWithKeyResponse r)
    {
        auto roomId = r.dataStr("roomId");
        auto type = r.dataStr("type");
        auto stateKey = r.dataStr("stateKey");

        if (! r.success()) {
            m.addTrigger(GetStateEventFailed{roomId, r.errorCode(), r.errorMessage()});
            return { std::move(m), lager::noop };
        }

        auto content = r.jsonBody();

        m.addTrigger(GetStateEventSuccessful{roomId, content});

        auto k = KeyOfState{type, stateKey};

        auto eventJson = m.roomList[roomId].stateEvents[k]
            .originalJson().get();

        eventJson["content"] = content;
        eventJson["type"] = type;
        eventJson["state_key"] = stateKey;

        auto a = AddStateEventsAction{EventList{Event{eventJson}}};

        auto l = RoomListModel::update(
            std::move(m.roomList),
            UpdateRoomAction{roomId, a});

        m.roomList = std::move(l);
        return { std::move(m), lager::noop };
    }


    ClientResult updateClient(ClientModel m, SendStateEventAction a)
    {
        auto event = a.event;

        if (event.type() == ""s) {
            m.addTrigger(InvalidMessageFormat{});
            return { std::move(m), lager::noop };
        }

        auto type = event.type();
        auto content = event.content();
        auto stateKey = event.stateKey();

        kzo.client.dbg() << "Sending state event of type " << type
                  << " with content " << content.get().dump()
                  << " to " << a.roomId
                  << " with state key #" << stateKey << std::endl;

        auto job = m.job<SetRoomStateWithKeyJob>().make(
            a.roomId,
            type,
            stateKey,
            content)
            .withData(json{
                    {"roomId", a.roomId},
                    {"eventType", type},
                    {"stateKey", stateKey},
                });

        m.addJob(std::move(job));

        return { std::move(m), lager::noop };
    }

    ClientResult processResponse(ClientModel m, SetRoomStateWithKeyResponse r)
    {
        auto roomId = r.dataStr("roomId");
        auto eventType = r.dataStr("eventType");
        auto stateKey = r.dataStr("stateKey");

        if (! r.success()) {
            kzo.client.dbg() << "Send state event failed" << std::endl;
            m.addTrigger(SendStateEventFailed{roomId, eventType, stateKey, r.errorCode(), r.errorMessage()});
            return { std::move(m), lager::noop };
        }

        m.addTrigger(SendStateEventSuccessful{roomId, r.eventId(), eventType, stateKey});

        return { std::move(m), lager::noop };
    }
}
