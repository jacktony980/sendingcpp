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
#include <zug/transducer/filter.hpp>

#include "debug.hpp"

#include "room-model.hpp"
#include "cursorutil.hpp"


namespace Kazv
{
    RoomModel RoomModel::update(RoomModel r, Action a)
    {
        return lager::match(std::move(a))(
            [&](AddStateEventsAction a) {
                r.stateEvents = merge(std::move(r.stateEvents), a.stateEvents, keyOfState);

                // If m.room.encryption state event appears,
                // configure the room to use encryption.
                if (r.stateEvents.find(KeyOfState{"m.room.encryption", ""})) {
                    auto newRoom = update(std::move(r), SetRoomEncryptionAction{});
                    r = std::move(newRoom);
                }
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
            },
            [&](AddEphemeralAction a) {
                r.ephemeral = merge(std::move(r.ephemeral), a.events, keyOfEphemeral);
                return r;
            },
            [&](SetLocalDraftAction a) {
                r.localDraft = a.localDraft;
                return r;
            },
            [&](SetRoomEncryptionAction) {
                r.encrypted = true;
                return r;
            },
            [&](MarkMembersFullyLoadedAction) {
                r.membersFullyLoaded = true;
                return r;
            }
            );
    }

    RoomListModel RoomListModel::update(RoomListModel l, Action a)
    {
        return lager::match(std::move(a))(
            [&](UpdateRoomAction a) {
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

    immer::flex_vector<std::string> RoomModel::joinedMemberIds() const
    {
        using MemberNode = std::pair<std::string, Kazv::Event>;

        auto memberNameTransducer =
            zug::filter(
                [](auto val) {
                    auto [k, v] = val;
                    auto [type, stateKey] = k;
                    return type == "m.room.member"s;
                })
            | zug::map(
                [](auto val) {
                    auto [k, v] = val;
                    auto [type, stateKey] = k;
                    return MemberNode{stateKey, v};
                })
            | zug::filter(
                [](auto val) {
                    auto [stateKey, ev] = val;
                    return ev.content().get()
                        .at("membership"s) == "join"s;
                })
            | zug::map(
                [](auto val) {
                    auto [stateKey, ev] = val;
                    return stateKey;
                });

        return intoImmer(
            immer::flex_vector<std::string>{},
            memberNameTransducer,
            stateEvents);
    }

    static Timestamp defaultRotateMs = 604800000;
    static int defaultRotateMsgs = 100;

    MegOlmSessionRotateDesc RoomModel::sessionRotateDesc() const
    {
        auto k = KeyOfState{"m.room.encryption", ""};
        auto content = stateEvents[k].content().get();
        auto ms = content.contains("rotation_period_ms")
            ? content["rotation_period_ms"].get<Timestamp>()
            : defaultRotateMs;
        auto msgs = content.contains("rotation_period_msgs")
            ? content["rotation_period_msgs"].get<int>()
            : defaultRotateMsgs;
        return MegOlmSessionRotateDesc{ ms, msgs };
    }

    bool RoomModel::hasUser(std::string userId) const
    {
        try {
            auto ev = stateEvents.at(KeyOfState{"m.room.member", userId});
            if (ev.content().get().at("membership") == "join") {
                return true;
            }
        } catch (const std::exception &) {
            return false;
        }
        return false;
    }
}
