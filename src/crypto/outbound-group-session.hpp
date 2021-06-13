/*
 * Copyright (C) 2021 Tusooa Zhu <tusooa@kazv.moe>
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

#include <maybe.hpp>
#include <event.hpp>

#include "crypto-util.hpp"

namespace Kazv
{
    struct OutboundGroupSessionPrivate;

    class OutboundGroupSession
    {
    public:
        /**
         * @return The size of random data needed to construct an OutboundGroupSession.
         */
        static std::size_t constructRandomSize();

        explicit OutboundGroupSession();

        /**
         * Constructs an OutboundGroupSession from custom random data.
         *
         * @param random The random data to use. Must be of at least size
         * `constructRandomSize()`.
         * @param creationTime The creation time of this OutboundGroupSession.
         */
        OutboundGroupSession(RandomTag, RandomData random, Timestamp creationTime);

        OutboundGroupSession(const OutboundGroupSession &that);
        OutboundGroupSession(OutboundGroupSession &&that);
        OutboundGroupSession &operator=(const OutboundGroupSession &that);
        OutboundGroupSession &operator=(OutboundGroupSession &&that);
        ~OutboundGroupSession();

        std::string encrypt(std::string plainText);

        bool valid() const;

        std::string sessionKey();
        std::string initialSessionKey() const;
        std::string sessionId();

        int messageIndex();

        Timestamp creationTimeMs() const;
    private:
        friend void to_json(nlohmann::json &j, const OutboundGroupSession &s);
        friend void from_json(const nlohmann::json &j, OutboundGroupSession &s);
        std::unique_ptr<OutboundGroupSessionPrivate> m_d;
    };
}
