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

#pragma once

#include "session.hpp"

namespace Kazv
{
    struct SessionPrivate
    {
        SessionPrivate();
        SessionPrivate(OutboundSessionTag,
                       OlmAccount *acc,
                       std::string theirIdentityKey,
                       std::string theirOneTimeKey);
        SessionPrivate(InboundSessionTag,
                       OlmAccount *acc,
                       std::string theirIdentityKey,
                       std::string message);

        SessionPrivate(const SessionPrivate &that);
        ~SessionPrivate() = default;

        ByteArray sessionData;
        OlmSession *session{0};

        bool valid{false};

        std::size_t checkError(std::size_t code) const;

        std::string error() const { return olm_session_last_error(session); }
    };

}
