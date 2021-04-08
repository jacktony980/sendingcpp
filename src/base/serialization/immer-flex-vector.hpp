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

#include <boost/serialization/nvp.hpp>
#include <boost/serialization/split_free.hpp>

#include <immer/flex_vector.hpp>
#include <immer/flex_vector_transient.hpp>

namespace boost::serialization
{
    template<class Archive,
             class T,
             class MP,
             std::uint32_t B,
             std::uint32_t BL>
    void save(Archive &ar, const immer::flex_vector<T, MP, B, BL> &v, const unsigned int /* version */)
    {
        auto size = v.size();
        ar << BOOST_SERIALIZATION_NVP(size);

        for (const auto &i : v) {
            ar << i;
        }
    }

    template<class Archive,
             class T,
             class MP,
             std::uint32_t B,
             std::uint32_t BL>
    void load(Archive &ar, immer::flex_vector<T, MP, B, BL> &v, const unsigned int /* version */)
    {
        using VecT = immer::flex_vector<T, MP, B, BL>;
        using TransientT = decltype(v.transient());
        using SizeT = decltype(v.size());
        using ValueT = typename VecT::value_type;

        SizeT size{};
        ar >> BOOST_SERIALIZATION_NVP(size);
        TransientT transient = VecT(size).transient();

        for (auto i = SizeT{}; i != size; ++i) {
            ValueT val;
            ar >> val;
            transient.set(i, std::move(val));
        }

        v = transient.persistent();
    }

    template<class Archive,
             class T,
             class MP,
             std::uint32_t B,
             std::uint32_t BL>
    inline void serialize(Archive &ar, immer::flex_vector<T, MP, B, BL> &v, const unsigned int version)
    {
        boost::serialization::split_free(ar, v, version);
    }
}
