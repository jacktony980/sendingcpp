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
#include <libkazv-config.hpp>

#include <memory>
#include <tuple>

#include <olm/olm.h>

#include <maybe.hpp>

#include "crypto-util.hpp"

namespace Kazv
{
    class Crypto;

    struct InboundSessionTag {};
    struct OutboundSessionTag {};

    struct SessionPrivate;
    class Session
    {
        // Creates an outbound session
        explicit Session(OutboundSessionTag,
                         OlmAccount *acc,
                         std::string theirIdentityKey,
                         std::string theirOneTimeKey);
        // Creates an inbound session
        explicit Session(InboundSessionTag,
                         OlmAccount *acc,
                         std::string theirIdentityKey,
                         std::string message);

    public:
        explicit Session();

        Session(const Session &that);
        Session(Session &&that);
        Session &operator=(const Session &that);
        Session &operator=(Session &&that);
        ~Session();

        bool matches(std::string message);

        bool valid() const;

        MaybeString decrypt(int type, std::string message);

        std::pair<int /* type */, std::string /* message */> encrypt(std::string plainText);

    private:
        friend class Crypto;
        friend class CryptoPrivate;

        friend void to_json(nlohmann::json &j, const Session &s);
        friend void from_json(const nlohmann::json &j, Session &s);
        std::unique_ptr<SessionPrivate> m_d;
    };
}
