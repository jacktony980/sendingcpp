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

#include <string>
#include <random>
#include <algorithm>
#include <vector>

#include <nlohmann/json.hpp>

#include <boost/container_hash/hash.hpp>

namespace Kazv
{
    using ByteArray = std::vector<unsigned char>;

    struct KeyOfGroupSession
    {
        std::string roomId;
        std::string senderKey;
        std::string sessionId;
    };

    inline void from_json(const nlohmann::json &j, KeyOfGroupSession &k)
    {
        k.roomId = j.at("roomId");
        k.senderKey = j.at("senderKey");
        k.sessionId = j.at("sessionId");
    }

    inline void to_json(nlohmann::json &j, const KeyOfGroupSession &k)
    {
        j = nlohmann::json::object({
                {"roomId", k.roomId},
                {"senderKey", k.senderKey},
                {"sessionId", k.sessionId},
            });
    }

    inline bool operator==(KeyOfGroupSession a, KeyOfGroupSession b)
    {
        return a.roomId == b.roomId
            && a.senderKey == b.senderKey
            && a.sessionId == b.sessionId;
    }

    struct KeyOfOutboundSession
    {
        std::string userId;
        std::string deviceId;
    };

    inline bool operator==(KeyOfOutboundSession a, KeyOfOutboundSession b)
    {
        return a.userId == b.userId
            && a.deviceId == b.deviceId;
    };

    [[nodiscard]] inline ByteArray genRandom(int len)
    {
        auto rd = std::random_device{};
        auto ret = ByteArray(len, '\0');
        std::generate(ret.begin(), ret.end(), [&] { return rd(); });
        return ret;
    }

    namespace CryptoConstants
    {
        inline const std::string ed25519{"ed25519"};
        inline const std::string curve25519{"curve25519"};
        inline const std::string signedCurve25519{"signed_curve25519"};

        inline const std::string olmAlgo{"m.olm.v1.curve25519-aes-sha2"};
        inline const std::string megOlmAlgo{"m.megolm.v1.aes-sha2"};
    }
}

namespace std
{
    template<> struct hash<Kazv::KeyOfGroupSession>
    {
        std::size_t operator()(const Kazv::KeyOfGroupSession & k) const noexcept {
            std::size_t seed = 0;
            boost::hash_combine(seed, k.roomId);
            boost::hash_combine(seed, k.senderKey);
            boost::hash_combine(seed, k.sessionId);
            return seed;
        }
    };

    template<> struct hash<Kazv::KeyOfOutboundSession>
    {
        std::size_t operator()(const Kazv::KeyOfOutboundSession & k) const noexcept {
            std::size_t seed = 0;
            boost::hash_combine(seed, k.userId);
            boost::hash_combine(seed, k.deviceId);
            return seed;
        }
    };
}
