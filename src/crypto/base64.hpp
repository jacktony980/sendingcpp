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

namespace Kazv
{
    namespace Base64Opts
    {
        /**
         * Bit flags for encodeBase64 and decodeBase64
         */
        enum Base64Opts : int
        {
            none = 0x0, /// The default, not url-safe, unpadded
            urlSafe = 0x1, /// Url-safe
            padded = 0x2, /// Add padding
        };
    }
    /**
     * Encodes the original string to base64.
     *
     * @param original The original string to encode.
     * @param flags Options for the encoder. See Base64Opts.
     */
    std::string encodeBase64(std::string original, int flags = Base64Opts::none);

    /**
     * Decode base64 string.
     *
     * @param encoded The base64-encoded string.
     * @param flags Options for the decoder. See Base64Opts.
     */
    std::string decodeBase64(std::string encoded, int flags = Base64Opts::none);
}
