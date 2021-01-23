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

#include <libkazv-config.hpp>

#include "ephemeral.hpp"

namespace Kazv
{
    ClientResult updateClient(ClientModel m, SetTypingAction a)
    {
        auto job = m.job<SetTypingJob>().make(
            m.userId,
            a.roomId,
            a.typing,
            a.timeoutMs)
            .withData(json{{"roomId", a.roomId}});

        m.addJob(std::move(job));

        return { std::move(m), lager::noop };
    }

    ClientResult processResponse(ClientModel m, SetTypingResponse r)
    {
        auto roomId = r.dataStr("roomId");
        if (! r.success()) {
            m.addTrigger(SetTypingFailed{roomId, r.errorCode(), r.errorMessage()});

            return { std::move(m), lager::noop };
        }

        m.addTrigger(SetTypingSuccessful{roomId});

        return { std::move(m), lager::noop };
    }

    ClientResult updateClient(ClientModel m, PostReceiptAction a)
    {
        auto job = m.job<PostReceiptJob>().make(
            a.roomId,
            /* receiptType = */ "m.read"s,
            a.eventId)
            .withData(json{{"roomId", a.roomId}});

        m.addJob(std::move(job));

        return { std::move(m), lager::noop };
    }

    ClientResult processResponse(ClientModel m, PostReceiptResponse r)
    {
        auto roomId = r.dataStr("roomId");
        if (! r.success()) {
            m.addTrigger(PostReceiptFailed{roomId, r.errorCode(), r.errorMessage()});

            return { std::move(m), lager::noop };
        }

        m.addTrigger(PostReceiptSuccessful{roomId});

        return { std::move(m), lager::noop };
    }

    ClientResult updateClient(ClientModel m, SetReadMarkerAction a)
    {
        auto job = m.job<SetReadMarkerJob>().make(
            a.roomId,
            /* mFullyRead = */ a.eventId)
            .withData(json{{"roomId", a.roomId}});

        m.addJob(std::move(job));

        return { std::move(m), lager::noop };
    }

    ClientResult processResponse(ClientModel m, SetReadMarkerResponse r)
    {
        auto roomId = r.dataStr("roomId");
        if (! r.success()) {
            m.addTrigger(SetReadMarkerFailed{roomId, r.errorCode(), r.errorMessage()});

            return { std::move(m), lager::noop };
        }

        m.addTrigger(SetReadMarkerSuccessful{roomId});

        return { std::move(m), lager::noop };
    }
}
