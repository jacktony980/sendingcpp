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

#include <boost/serialization/string.hpp>
#include <boost/serialization/split_member.hpp>

#include <types.hpp>
#include <copy-helper.hpp>

namespace Kazv
{
    class EncryptedFileDesc
    {
    public:
        /**
         * Constructs an empty EncryptedFileDesc.
         *
         * The mxcUri, key, iv, and sha256Hash are guaranteed to be empty.
         */
        EncryptedFileDesc();

        /**
         * Constructs an EncryptedFileDesc from mxcUri, key, iv and sha256Hash.
         *
         * @param mxcUri The mxc uri to the file.
         * @param key The key encoded as urlsafe unpadded base64.
         * @param iv The iv encoded as unpadded base64.
         * @param sha256Hash The SHA256 hash encoded as unpadded base64.
         */
        EncryptedFileDesc(std::string mxcUri, std::string key, std::string iv, std::string sha256Hash);

        KAZV_DECLARE_COPYABLE(EncryptedFileDesc)

        ~EncryptedFileDesc();

        /**
         * Constructs an EncryptedFileDesc from an EncryptedFile json object.
         *
         * The EncryptedFile type is defined in
         * https://matrix.org/docs/spec/client_server/latest#id431 .
         *
         * @return An EncryptedFileDesc object representing the EncryptedFile object,
         * if `encryptedFile` is valid; a default-constructed EncryptedFileDesc otherwise.
         */
        static EncryptedFileDesc fromJson(JsonWrap encryptedFile);

        /**
         * Convert the encrypted file metadata to an EncryptedFile json object.
         *
         * @return An EncryptedFile json object representing this.
         */
        JsonWrap toJson() const;

        /// @return The mxc uri for this file.
        std::string mxcUri() const;
        /// @return The key encoded as urlsafe unpadded base64.
        std::string key() const;
        /// @return The iv encoded as unpadded base64.
        std::string iv() const;
        /// @return The SHA256 hash encoded as unpadded base64.
        std::string sha256Hash() const;

        /**
         * @return Whether all of mxcUri, key, iv, and sha256Hash of this
         * and that are equal.
         */
        bool operator==(const EncryptedFileDesc &that) const;

        template<class Archive>
        void save(Archive &ar, const unsigned int /* version */) const {
            ar << mxcUri()
               << key()
               << iv()
               << sha256Hash();
        }

        template<class Archive>
        void load(Archive &ar, const unsigned int /* version */) {
            std::string mxcUri;
            std::string key;
            std::string iv;
            std::string sha256Hash;
            ar >> mxcUri
               >> key
               >> iv
               >> sha256Hash;

            *this = EncryptedFileDesc(mxcUri, key, iv, sha256Hash);
        }

        BOOST_SERIALIZATION_SPLIT_MEMBER()

    private:
        struct Private;
        std::unique_ptr<Private> m_d;
    };
}
