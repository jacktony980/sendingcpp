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


#include <copy-helper.hpp>

#include "crypto-util.hpp"


namespace Kazv
{
    class SHA256Desc
    {
    public:
        using DataT = std::string;

        /**
         * Construct a SHA256 description.
         */
        SHA256Desc();

        KAZV_DECLARE_COPYABLE(SHA256Desc)

        ~SHA256Desc();

        /**
         * Process data and return the next hash state.
         */
        SHA256Desc process(DataT data) const &;

        template<class RangeT,
                 class = std::enable_if_t<!std::is_same_v<std::decay_t<RangeT>, DataT>, int>>
        SHA256Desc process(RangeT data) const &
        {
            return process(DataT(data.begin(), data.end()));
        }

        /**
         * Process data and return the next hash state.
         */
        SHA256Desc process(DataT data) &&;

        template<class RangeT,
                 class = std::enable_if_t<!std::is_same_v<std::decay_t<RangeT>, DataT>, int>>
        SHA256Desc process(RangeT data) &&
        {
            return std::move(*this).process(DataT(data.begin(), data.end()));
        }

        /**
         * Process data in-place.
         */
        void processInPlace(DataT data);

        template<class RangeT,
                 class = std::enable_if_t<!std::is_same_v<std::decay_t<RangeT>, DataT>, int>>
        void processInPlace(RangeT data)
        {
            processInPlace(DataT(data.begin(), data.end()));
        }

        /**
         * Get the hash result as unpadded base64.
         */
        std::string get() const;

    private:
        struct Private;
        std::unique_ptr<Private> m_d;
    };
}
