/*
 * Copyright (C) 2020-2021 Tusooa Zhu <tusooa@vista.aero>
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

#include <memory>

#include <nlohmann/json.hpp>

#include <immer/map.hpp>
#include <immer/flex_vector.hpp>

#include <maybe.hpp>

#include "crypto-util.hpp"
#include "time-util.hpp"

namespace Kazv
{
    class Session;

    struct MegOlmSessionRotateDesc
    {
        Timestamp ms{};
        int messages{};
    };

    struct CryptoPrivate;
    class Crypto
    {
    public:
        explicit Crypto();
        Crypto(const Crypto &that);
        Crypto(Crypto &&that);
        Crypto &operator=(const Crypto &that);
        Crypto &operator=(Crypto &&that);
        ~Crypto();

        std::string ed25519IdentityKey();
        std::string curve25519IdentityKey();

        std::string sign(nlohmann::json j);

        void setUploadedOneTimeKeysCount(immer::map<std::string /* algorithm */, int> uploadedOneTimeKeysCount);

        int uploadedOneTimeKeysCount(std::string algorithm) const;

        int maxNumberOfOneTimeKeys();

        void genOneTimeKeys(int num);

        /**
         * According to olm.h, this returns an object like
         *
         * {
         *     curve25519: {
         *         "AAAAAA": "wo76WcYtb0Vk/pBOdmduiGJ0wIEjW4IBMbbQn7aSnTo",
         *         "AAAAAB": "LRvjo46L1X2vx69sS9QNFD29HWulxrmW11Up5AfAjgU"
         *     }
         * }
         */
        nlohmann::json unpublishedOneTimeKeys();

        int numUnpublishedOneTimeKeys() const;

        void markOneTimeKeysAsPublished();

        /// Returns decrypted message if we can decrypt it
        /// otherwise returns the error
        MaybeString decrypt(nlohmann::json eventJson);


        /** returns a json object that looks like
         * {
         *   "<their identity key>": {
         *     "type": <number>,
         *     "body": "<body>"
         *   }
         * }
         */
        nlohmann::json encryptOlm(nlohmann::json eventJson, std::string theirCurve25519IdentityKey);

        /// returns the content template with everything but deviceId
        /// eventJson should contain type, room_id and content
        nlohmann::json encryptMegOlm(nlohmann::json eventJson);

        bool createInboundGroupSession(KeyOfGroupSession k, std::string sessionKey, std::string ed25519Key);

        std::string outboundGroupSessionInitialKey(std::string roomId);

        std::string outboundGroupSessionCurrentKey(std::string roomId);

        /// Check whether the signature of userId/deviceId is valid in object
        bool verify(nlohmann::json object, std::string userId, std::string deviceId, std::string ed25519Key);

        MaybeString getInboundGroupSessionEd25519KeyFromEvent(const nlohmann::json &eventJson) const;

        /// Returns the new session key
        std::string rotateMegOlmSession(std::string roomId);

        /// Returns the new session key only if it is rotated
        std::optional<std::string> rotateMegOlmSessionIfNeeded(std::string roomId, MegOlmSessionRotateDesc desc);

        using UserIdToDeviceIdMap = immer::map<std::string, immer::flex_vector<std::string>>;
        UserIdToDeviceIdMap devicesMissingOutboundSessionKey(
            immer::map<std::string, immer::map<std::string /* deviceId */,
            std::string /* curve25519IdentityKey */>> keyMap) const;

        void createOutboundSession(std::string theirIdentityKey,
                                   std::string theirOneTimeKey);

    private:
        friend class Session;
        friend class SessionPrivate;
        std::unique_ptr<CryptoPrivate> m_d;
    };
}
