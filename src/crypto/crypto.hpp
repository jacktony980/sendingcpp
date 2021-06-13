/*
 * Copyright (C) 2020-2021 Tusooa Zhu <tusooa@kazv.moe>
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
        /// The time limit of the megolm session, in milliseconds.
        Timestamp ms{};
        /// The message limit of the megolm session.
        int messages{};
    };

    struct CryptoPrivate;
    class Crypto
    {
    public:
        [[deprecated("Use deterministic variant instead. In the future, this will construct an invalid Crypto.")]]
        explicit Crypto();

        /**
         * @return The size of random data needed to construct a Crypto.
         */
        static std::size_t constructRandomSize();

        /**
         * Constructs a Crypto using user-provided random data.
         *
         * @param data Random data of size at least `constructRandomSize()`.
         */
        Crypto(RandomTag, RandomData data);

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

        /**
         * Get the size of random data needed to generate `num`
         * one-time keys.
         *
         * @param num The number of one-time keys to generate.
         *
         * @return The size of random data needed to generate
         * `num` one-time keys.
         */
        static std::size_t genOneTimeKeysRandomSize(int num);

        [[deprecated("Use the deterministic variant. In the future, this will be removed.")]]
        void genOneTimeKeys(int num);

        /**
         * Generate `num` one-time keys with user-provided random data.
         *
         * @param random The random data. Must be of at least size
         * `genOneTimeKeysRandomSize(num)`.
         * @param num The number of one-time keys to generate.
         */
        void genOneTimeKeysWithRandom(RandomData random, int num);

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

        /**
         * @return The size of random data needed to encrypt a message
         * for the session identified with `theirCurve25519IdentityKey`.
         */
        std::size_t encryptOlmRandomSize(std::string theirCurve25519IdentityKey) const;

        /** returns a json object that looks like
         * {
         *   "<their identity key>": {
         *     "type": <number>,
         *     "body": "<body>"
         *   }
         * }
         */
        [[deprecated("Use deterministic variant instead. In the future, this will be removed.")]]
        nlohmann::json encryptOlm(nlohmann::json eventJson, std::string theirCurve25519IdentityKey);

        /**
         * Encrypt `eventJson` with olm, for the recipient identified with `theirCurve25519IdentityKey`.
         *
         * @param random The random data to use for encryption. Must be of
         * at least size `encryptOlmRandomSize(theirCurve25519IdentityKey)`.
         * @param eventJson The event json to encrypt.
         * @param theirCurve25519IdentityKey The curve25519 identity key of the recipient.
         *
         * @return A json object that looks like
         * ```
         * {
         *   "<their identity key>": {
         *     "type": <number>,
         *     "body": "<body>"
         *   }
         * }
         * ```
         */
        nlohmann::json encryptOlmWithRandom(
            RandomData random, nlohmann::json eventJson, std::string theirCurve25519IdentityKey);

        /// returns the content template with everything but deviceId
        /// eventJson should contain type, room_id and content
        nlohmann::json encryptMegOlm(nlohmann::json eventJson);

        bool createInboundGroupSession(KeyOfGroupSession k, std::string sessionKey, std::string ed25519Key);

        std::string outboundGroupSessionInitialKey(std::string roomId);

        std::string outboundGroupSessionCurrentKey(std::string roomId);

        /// Check whether the signature of userId/deviceId is valid in object
        bool verify(nlohmann::json object, std::string userId, std::string deviceId, std::string ed25519Key);

        MaybeString getInboundGroupSessionEd25519KeyFromEvent(const nlohmann::json &eventJson) const;

        /**
         * @return The size of random data needed for `rotateMegOlmSessionWithRandom()`
         * and `rotateMegOlmSessionWithRandomIfNeeded()`.
         */
        static std::size_t rotateMegOlmSessionRandomSize();

        /// Returns the new session key
        [[deprecated("Use deterministic variant instead. In the future, this will be removed.")]]
        std::string rotateMegOlmSession(std::string roomId);

        /**
         * Rotate the megolm session using user-provided random data.
         *
         * @param random The random data. Must be of at least size
         * `rotateMegOlmSessionRandomSize()`.
         * @param timeMs The creation time of the new megolm session.
         * @param roomId The room id of the megolm session to rotate.
         *
         * @return The new session key.
         */
        std::string rotateMegOlmSessionWithRandom(RandomData random, Timestamp timeMs, std::string roomId);

        /// Returns the new session key only if it is rotated
        [[deprecated("Use deterministic variant instead. In the future, this will be removed.")]]
        std::optional<std::string> rotateMegOlmSessionIfNeeded(std::string roomId, MegOlmSessionRotateDesc desc);

        /**
         * Rotate the megolm session using user-provided random data,
         * if we need to rotate it.
         *
         * The session will be rotated if and only if
         * - The difference between `timeMs` and the creation time of
         *   the megolm session has reached the time limit in `desc`, OR;
         * - The number of messages this megolm session has encrypted has
         *   reached the message limit in `desc`.
         *
         * @param random The random data. Must be of at least size
         * `rotateMegOlmSessionRandomSize()`.
         * @param timeMs The timestamp to judge whether the session
         * has reached its time limit. If the megolm session is rotated,
         * this will also be the creation time of the new megolm session.
         * @param roomId The room id of the megolm session to rotate.
         * @param desc The rotation specification of this room.
         *
         * @return The session key if the session is rotated,
         * `std::nullopt` otherwise.
         */
        std::optional<std::string> rotateMegOlmSessionWithRandomIfNeeded(
            RandomData random,
            Timestamp timeMs,
            std::string roomId,
            MegOlmSessionRotateDesc desc);

        using UserIdToDeviceIdMap = immer::map<std::string, immer::flex_vector<std::string>>;
        UserIdToDeviceIdMap devicesMissingOutboundSessionKey(
            immer::map<std::string, immer::map<std::string /* deviceId */,
            std::string /* curve25519IdentityKey */>> keyMap) const;

        [[deprecated("Use deterministic variant instead. In the future, this will be removed.")]]
        void createOutboundSession(std::string theirIdentityKey,
                                   std::string theirOneTimeKey);

        /**
         * @return The size of random data needed for `createOutboundSessionWithRandom()`.
         */
        static std::size_t createOutboundSessionRandomSize();

        /**
         * Create an outbound session using user-provided random data.
         *
         * @param random The random data to use. It must be at least of
         * size `createOutboundSessionRandomSize()`.
         * @param theirIdeneityKey The identity key of the recipient.
         * @param theirOneTimeKey The one-time key of the recipient.
         */
        void createOutboundSessionWithRandom(
            RandomData random,
            std::string theirIdentityKey,
            std::string theirOneTimeKey);

        template<class Archive>
        void save(Archive & ar, const unsigned int /* version */) const {
            ar << toJson().dump();
        }

        template<class Archive>
        void load(Archive &ar, const unsigned int /* version */) {
            std::string j;
            ar >> j;
            loadJson(nlohmann::json::parse(std::move(j)));
        }

        BOOST_SERIALIZATION_SPLIT_MEMBER()

        nlohmann::json toJson() const;
        void loadJson(const nlohmann::json &j);

    private:

        friend class Session;
        friend class SessionPrivate;
        std::unique_ptr<CryptoPrivate> m_d;
    };
}

BOOST_CLASS_VERSION(Kazv::Crypto, 0)
