/*
 * Copyright (C) 2021 Tusooa Zhu <tusooa@vista.aero>
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

#include "room.hpp"

namespace Kazv
{
    Room::Room(lager::reader<SdkModel> sdk,
               lager::reader<std::string> roomId,
               Context<ClientAction> ctx)
        : m_sdk(sdk)
        , m_room(lager::with(m_sdk.map(&SdkModel::c)[&ClientModel::roomList], roomId)
                 .map([](auto rooms, auto id) {
                          return rooms[id];
                      }).make())
        , m_ctx(ctx)
    {
    }

    auto Room::setLocalDraft(std::string localDraft) const
        -> PromiseT
    {
        using namespace CursorOp;
        return m_ctx.dispatch(UpdateRoomAction{+roomId(), SetLocalDraftAction{localDraft}});
    }

    auto Room::sendMessage(Event msg) const
        -> PromiseT
    {
        using namespace CursorOp;
        auto hasCrypto = ~m_sdk.map([](const auto &sdk) -> bool {
                                        return sdk.c().crypto.has_value();
                                    });
        auto roomEncrypted = ~m_room[&RoomModel::encrypted];
        auto noFullMembers = ~m_room[&RoomModel::membersFullyLoaded]
            .map([](auto b) { return !b; });

        auto rid = +roomId();

        // Don't use m_ctx directly in the callbacks
        // as `this` may have been destroyed when
        // the callbacks are called.
        auto ctx = m_ctx;

        auto promise = ctx.createResolvedPromise(true);
        if (+allCursors(hasCrypto, roomEncrypted, noFullMembers)) {
            kzo.client.dbg() << "The members of " << rid
                             << " are not fully loaded." << std::endl;

            promise = promise
                .then([=](auto) {
                          return ctx.dispatch(GetRoomStatesAction{rid});
                      })
                .then([=](auto succ) {
                          if (! succ) {
                              kzo.client.warn() << "Loading members of " << rid
                                                << " failed." << std::endl;
                              return ctx.createResolvedPromise(false);
                          } else {
                              // XXX remove the hard-coded initialSync parameter
                              return ctx.dispatch(QueryKeysAction{true})
                                  .then([](auto succ) {
                                            if (! succ) {
                                                kzo.client.warn() << "Query keys failed" << std::endl;
                                            }
                                            return succ;
                                        });
                          }
                      });
        }

        return promise
            .then([=](auto succ) {
                      if (! succ) {
                          return ctx.createResolvedPromise(false);
                      }
                      return ctx.dispatch(SendMessageAction{rid, msg});
                  });
    }

    auto Room::sendTextMessage(std::string text) const
        -> PromiseT
    {
        json j{
            {"type", "m.room.message"},
            {"content", {
                    {"msgtype", "m.text"},
                    {"body", text}
                }
            }
        };
        Event e{j};
        return sendMessage(e);
    }

    auto Room::refreshRoomState() const
        -> PromiseT
    {
        using namespace CursorOp;
        return m_ctx.dispatch(GetRoomStatesAction{+roomId()});
    }

    auto Room::getStateEvent(std::string type, std::string stateKey) const
        -> PromiseT
    {
        using namespace CursorOp;
        return m_ctx.dispatch(GetStateEventAction{+roomId(), type, stateKey});
    }

    auto Room::sendStateEvent(Event state) const
        -> PromiseT
    {
        using namespace CursorOp;
        return m_ctx.dispatch(SendStateEventAction{+roomId(), state});
    }

    auto Room::setName(std::string name) const
        -> PromiseT
    {
        json j{
            {"type", "m.room.name"},
            {"content", {
                    {"name", name}
                }
            }
        };
        Event e{j};
        return sendStateEvent(e);
    }

    auto Room::setTopic(std::string topic) const
        -> PromiseT
    {
        json j{
            {"type", "m.room.topic"},
            {"content", {
                    {"topic", topic}
                }
            }
        };
        Event e{j};
        return sendStateEvent(e);
    }

    auto Room::invite(std::string userId) const
        -> PromiseT
    {
        using namespace CursorOp;
        return m_ctx.dispatch(InviteToRoomAction{+roomId(), userId});
    }

    auto Room::setTyping(bool typing, std::optional<int> timeoutMs) const
        -> PromiseT
    {
        using namespace CursorOp;
        return m_ctx.dispatch(SetTypingAction{+roomId(), typing, timeoutMs});
    }

    auto Room::leave() const
        -> PromiseT
    {
        using namespace CursorOp;
        return m_ctx.dispatch(LeaveRoomAction{+roomId()});
    }

    auto Room::forget() const
        -> PromiseT
    {
        using namespace CursorOp;
        return m_ctx.dispatch(ForgetRoomAction{+roomId()});
    }

    auto Room::setPinnedEvents(immer::flex_vector<std::string> eventIds) const
        -> PromiseT
    {
        json j{
            {"type", "m.room.pinned_events"},
            {"content", {
                    {"pinned", eventIds}
                }
            }
        };
        Event e{j};
        return sendStateEvent(e);
    }

    auto Room::timelineGaps() const
        -> lager::reader<immer::map<std::string /* eventId */,
                                    std::string /* prevBatch */>>
    {
        return m_room[&RoomModel::timelineGaps];
    }

    auto Room::paginateBackFromEvent(std::string eventId) const
        -> PromiseT
    {
        using namespace CursorOp;
        return m_ctx.dispatch(PaginateTimelineAction{
                +roomId(), eventId, std::nullopt});
    }
}
