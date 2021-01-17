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


#pragma once

#include <lager/debug/cereal/struct.hpp>
#include <lager/debug/cereal/immer_flex_vector.hpp>

#include <string>
#include <variant>
#include <immer/flex_vector.hpp>
#include <immer/map.hpp>

#include <csapi/sync.hpp>
#include <event.hpp>

#include "data/cereal_map.hpp"

#include "clientutil.hpp"

namespace Kazv
{
    struct AddStateEventsAction
    {
        immer::flex_vector<Event> stateEvents;
    };

    struct AppendTimelineAction
    {
        immer::flex_vector<Event> events;
    };

    struct PrependTimelineAction
    {
        immer::flex_vector<Event> events;
        std::string paginateBackToken;
    };

    struct AddAccountDataAction
    {
        immer::flex_vector<Event> events;
    };

    struct ChangeMembershipAction
    {
        RoomMembership membership;
    };

    struct ChangeInviteStateAction
    {
        immer::flex_vector<Event> events;
    };

    struct AddEphemeralAction
    {
        EventList events;
    };

    struct SetLocalDraftAction
    {
        std::string localDraft;
    };

    struct SetRoomEncryptionAction
    {
    };

    struct RoomModel
    {
        using Membership = RoomMembership;

        std::string roomId;
        immer::map<KeyOfState, Event> stateEvents;
        immer::map<KeyOfState, Event> inviteState;
        immer::flex_vector<std::string> timeline;
        immer::map<std::string, Event> messages;
        immer::map<std::string, Event> accountData;
        Membership membership{};
        std::string paginateBackToken;
        /// whether this room has earlier events to be fetched
        bool canPaginateBack{true};

        immer::map<std::string, Event> ephemeral;

        std::string localDraft;

        bool encrypted{false};

        using Action = std::variant<
            AddStateEventsAction,
            AppendTimelineAction,
            PrependTimelineAction,
            AddAccountDataAction,
            ChangeMembershipAction,
            ChangeInviteStateAction,
            AddEphemeralAction,
            SetLocalDraftAction,
            SetRoomEncryptionAction
            >;

        static RoomModel update(RoomModel r, Action a);
    };

    using RoomAction = RoomModel::Action;

    inline bool operator==(RoomModel a, RoomModel b)
    {
        return a.roomId == b.roomId
            && a.stateEvents == b.stateEvents
            && a.inviteState == b.inviteState
            && a.timeline == b.timeline
            && a.messages == b.messages
            && a.accountData == b.accountData
            && a.membership == b.membership
            && a.paginateBackToken == b.paginateBackToken
            && a.canPaginateBack == b.canPaginateBack
            && a.ephemeral == b.ephemeral
            && a.localDraft == b.localDraft
            && a.encrypted == b.encrypted;
    }

    struct UpdateRoomAction
    {
        std::string roomId;
        RoomAction roomAction;
    };

    struct RoomListModel
    {
        immer::map<std::string, RoomModel> rooms;

        inline auto at(std::string id) const { return rooms.at(id); }
        inline auto operator[](std::string id) const { return rooms[id]; }
        inline bool has(std::string id) const { return rooms.find(id); }

        using Action = std::variant<
            UpdateRoomAction
            >;
        static RoomListModel update(RoomListModel l, Action a);
    };

    using RoomListAction = RoomListModel::Action;

    inline bool operator==(RoomListModel a, RoomListModel b)
    {
        return a.rooms == b.rooms;
    }

#ifndef NDEBUG
    LAGER_CEREAL_STRUCT(AddStateEventsAction);
    LAGER_CEREAL_STRUCT(AppendTimelineAction);
    LAGER_CEREAL_STRUCT(PrependTimelineAction);
    LAGER_CEREAL_STRUCT(AddAccountDataAction);
    LAGER_CEREAL_STRUCT(ChangeMembershipAction);
    LAGER_CEREAL_STRUCT(SetLocalDraftAction);
    LAGER_CEREAL_STRUCT(ChangeInviteStateAction);
    LAGER_CEREAL_STRUCT(UpdateRoomAction);
#endif

    template<class Archive>
    void serialize(Archive &ar, RoomModel &r, std::uint32_t const /*version*/)
    {
        ar(r.roomId,
           r.stateEvents,
           r.inviteState,
           r.timeline,
           r.messages,
           r.accountData,
           r.membership,
           r.paginateBackToken,
           r.canPaginateBack);
    }

    template<class Archive>
    void serialize(Archive &ar, RoomListModel &l, std::uint32_t const /*version*/)
    {
        ar(l.rooms);
    }
}

CEREAL_CLASS_VERSION(Kazv::RoomModel, 0);
CEREAL_CLASS_VERSION(Kazv::RoomListModel, 0);
