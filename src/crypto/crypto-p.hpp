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

#include <olm/olm.h>

#include <map>

#include "crypto.hpp"
#include "crypto-util.hpp"
#include "session.hpp"

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
        std::map<std::string /* theirCurve25519IdentityKey */, Session> knownSessions;

        ByteArray identityKeys();
        std::string ed25519IdentityKey();
        std::string curve25519IdentityKey();

        std::size_t checkError(std::size_t code) const;

        MaybeString decryptOlm(nlohmann::json content);
        MaybeString decryptMegOlm(nlohmann::json content);

        /// returns whether the session is successfully established
        bool createInboundSession(std::string theirCurve25519IdentityKey,
                                  std::string message);
    };

}