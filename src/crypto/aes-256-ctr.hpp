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


#include <copy-helper.hpp>

#include "crypto-util.hpp"


namespace Kazv
{
    namespace
    {
        using std::begin;
        using std::end;
    }

    class AES256CTRDesc
    {
    public:
        using SecureData = std::string;

        /// The optimal block size for this cipher
        inline static const int optimalBlockSize{16};
        /// The key size for this cipher
        inline static const int keySize{32};
        /// The iv size for this cipher
        inline static const int ivSize{16}; // AES block size
        using DataT = std::string;

        /**
         * Constructs an AES-256-CTR cipher using `key` and `iv`.
         *
         * `key` must be of size `keySize` and `iv` must be of size `ivSize`.
         * Otherwise, the constructed cipher is invalid.
         */
        AES256CTRDesc(SecureData key, DataT iv);

        /**
         * Constructs an AES-256-CTR cipher using `key` and `iv` of any range type.
         *
         * `key` must be of size `keySize` and `iv` must be of size `ivSize`.
         * Otherwise, the constructed cipher is invalid.
         *
         * This overload does not participate in resolution if Key is SecureData
         * and Iv is DataT.
         */
        template<class Key, class Iv,
                 class = std::enable_if_t<!(std::is_same_v<std::decay_t<Key>, SecureData>
                                            && std::is_same_v<std::decay_t<Iv>, DataT>), int>>
        AES256CTRDesc(Key key, Iv iv)
            : AES256CTRDesc(SecureData(begin(key), end(key)), DataT(begin(iv), end(iv))) {}


        KAZV_DECLARE_COPYABLE(AES256CTRDesc)

        ~AES256CTRDesc();

        /**
         * @return whether this cipher is valid. If the cipher is invalid, you
         * must not use any method except destructor or operator=().
         */
        bool valid();

        template<class RangeT>
        using ResultBase = std::pair<AES256CTRDesc, RangeT>;

        using Result = ResultBase<DataT>;

        /**
         * Encrypts or decrypts `data` and derives the next cipher state.
         *
         * @param data The data to encrypt or decrypt.
         *
         * Example:
         *
         *     auto cipher = Kazv::AES256CTRDesc(
         *         Kazv::genRandom(Kazv::AES256CTRDesc::keySize),
         *         Kazv::genRandom(Kazv::AES256CTRDesc::ivSize)
         *     );
         *     std::string original = "Some test here";
         *     auto [next, encrypted] = cipher.process(original);
         *     auto [next2, encrypted2] = cipher.process(original);
         *     REQUIRE(encrypted == encrypted2);
         *
         * @return an object that can be destructured into the next cipher state
         * (as AES256CTRDesc) and the encrypted or decrypted data (as DataT).
         */
        Result process(DataT data) const &;
        /**
         * Encrypts or decrypts `data` and derives the next cipher state.
         *
         * This function only differs from the above in that it takes any range type
         * and the second component of returned object is of the same type as the argument.
         */
        template<class RangeT,
                 class = std::enable_if_t<!std::is_same_v<std::decay_t<RangeT>, DataT>, int>>
        ResultBase<std::decay_t<RangeT>> process(RangeT data) const & {
            using ActualRangeT = std::decay_t<RangeT>;
            auto [next, res] = process(DataT(begin(data), end(data)));
            return { next, ActualRangeT(res.begin(), res.end()) };
        }

        /**
         * Encrypts or decrypts `data` and derives the next cipher state.
         *
         * This function differs from the above two only in that its calling object
         * is an rvalue reference (a.k.a casted using std::move()).
         */
        Result process(DataT data) &&;
        /**
         * Encrypts or decrypts `data` and derives the next cipher state.
         *
         * This function differs from the above only in that it takes any range type
         * and the second component of returned object is of the same type as the argument.
         */
        template<class RangeT,
                 class = std::enable_if_t<!std::is_same_v<std::decay_t<RangeT>, DataT>, int>>
        ResultBase<std::decay_t<RangeT>> process(RangeT data) && {
            using ActualRangeT = std::decay_t<RangeT>;
            auto [next, res] = std::move(*this).process(DataT(begin(data), end(data)));
            return { next, ActualRangeT(res.begin(), res.end()) };
        }

        /**
         * Encrypt or decrypt `data` and modify the state of the cipher in-place.
         *
         * @param data The data to encrypt or decrypt.
         */
        DataT processInPlace(DataT data);
        /**
         * Encrypt or decrypt `data` and modify the state of the cipher in-place.
         *
         * This function differs from the above only in that it takes any range type
         * and returns the processed data as the same range type.
         */
        template<class RangeT,
                 class = std::enable_if_t<!std::is_same_v<std::decay_t<RangeT>, DataT>, int>>
        std::decay_t<RangeT> processInPlace(RangeT data) {
            using ActualRangeT = std::decay_t<RangeT>;
            auto res = processInPlace(DataT(begin(data), end(data)));
            return ActualRangeT(res.begin(), res.end());
        }

    private:
        struct Private;

        std::unique_ptr<Private> m_d;
    };
}
