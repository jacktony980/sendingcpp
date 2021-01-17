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

#include <maybe.hpp>

#include "crypto-util.hpp"


namespace Kazv
{
    class Session;

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
        MaybeString decrypt(nlohmann::json content);

    private:
        friend class Session;
        friend class SessionPrivate;
        std::unique_ptr<CryptoPrivate> m_d;
    };
}
