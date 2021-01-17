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

#include <olm/olm.h>

#include <unordered_map>

#include "crypto.hpp"
#include "crypto-util.hpp"
#include "session.hpp"
#include "inbound-group-session.hpp"

namespace Kazv
{
    using SessionList = std::vector<Session>;

    struct CryptoPrivate
    {
        CryptoPrivate();
        CryptoPrivate(const CryptoPrivate &that);
        ~CryptoPrivate();

        ByteArray accountData;
        OlmAccount *account;
        immer::map<std::string /* algorithm */, int> uploadedOneTimeKeysCount;
        int numUnpublishedKeys{0};
        std::unordered_map<std::string /* theirCurve25519IdentityKey */, Session> knownSessions;
        std::unordered_map<KeyOfGroupSession, InboundGroupSession> inboundGroupSessions;

        ByteArray utilityData;
        OlmUtility *utility;

        std::size_t checkUtilError(std::size_t code) const;

        ByteArray pickle() const;
        void unpickle(ByteArray data);

        ByteArray identityKeys();
        std::string ed25519IdentityKey();
        std::string curve25519IdentityKey();

        std::size_t checkError(std::size_t code) const;

        MaybeString decryptOlm(nlohmann::json content);
        // Here we need the full event for eventId and originServerTs
        MaybeString decryptMegOlm(nlohmann::json eventJson);

        /// returns whether the session is successfully established
        bool createInboundSession(std::string theirCurve25519IdentityKey,
                                  std::string message);
    };

}
