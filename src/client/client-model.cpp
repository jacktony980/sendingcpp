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


#include <lager/util.hpp>
#include <lager/context.hpp>
#include <functional>

#include <immer/flex_vector_transient.hpp>

#include "client-model.hpp"

#include "actions/states.hpp"
#include "actions/auth.hpp"
#include "actions/membership.hpp"
#include "actions/paginate.hpp"
#include "actions/send.hpp"
#include "actions/states.hpp"
#include "actions/sync.hpp"
#include "actions/ephemeral.hpp"
#include "actions/content.hpp"

namespace Kazv
{
    auto ClientModel::update(ClientModel m, Action a) -> Result
    {
        return lager::match(std::move(a))(
            [&](Error::Action a) -> Result {
                m.error = Error::update(m.error, a);
                return {std::move(m), lager::noop};
            },

            [&](RoomListAction a) -> Result {
                m.roomList = RoomListModel::update(std::move(m.roomList), a);
                return {std::move(m), lager::noop};
            },
            [&](ResubmitJobAction a) -> Result {
                m.addJob(std::move(a.job));
                return { std::move(m), lager::noop };
            },
            [&](auto a) -> decltype(updateClient(m, a)) {
                return updateClient(m, a);
            },
#define RESPONSE_FOR(_jobId)                                            \
            if (r.jobId() == #_jobId) {                                 \
                return processResponse(m, _jobId##Response{std::move(r)}); \
            }

            [&](ProcessResponseAction a) -> Result {
                auto r = std::move(a.response);

                // auth
                RESPONSE_FOR(Login);
                // paginate
                RESPONSE_FOR(GetRoomEvents);
                // sync
                RESPONSE_FOR(Sync);
                // membership
                RESPONSE_FOR(CreateRoom);
                RESPONSE_FOR(InviteUser);
                RESPONSE_FOR(JoinRoomById);
                RESPONSE_FOR(JoinRoom);
                RESPONSE_FOR(LeaveRoom);
                RESPONSE_FOR(ForgetRoom);
                // send
                RESPONSE_FOR(SendMessage);
                // states
                RESPONSE_FOR(GetRoomState);
                RESPONSE_FOR(SetRoomStateWithKey);
                // ephemeral
                RESPONSE_FOR(SetTyping);
                RESPONSE_FOR(PostReceipt);
                RESPONSE_FOR(SetReadMarker);
                // content
                RESPONSE_FOR(UploadContent);
                RESPONSE_FOR(GetContent);
                RESPONSE_FOR(GetContentThumbnail);

                m.addTrigger(UnrecognizedResponse{r});
                return { m, lager::noop };
            }

#undef RESPONSE_FOR
            );
    }
}
