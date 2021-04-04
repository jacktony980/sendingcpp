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

#include <immer/map.hpp>

namespace boost::serialization
{

    template <class Archive, class K, class T, class H, class E, class MP>
    void save(Archive &ar, const immer::map<K, T, H, E, MP> &map, const unsigned int /* version */)
    {
        auto size = map.size();
        ar << BOOST_SERIALIZATION_NVP(size);
        for (const auto &[k, v] : map) {
            ar << k << v;
        }
    }

    template <class Archive, class K, class T, class H, class E, class MP>
    void load(Archive &ar, immer::map<K, T, H, E, MP> &map, const unsigned int /* version */)
    {
        // map transient is not yet implemented
        // using TransientT = decltype(v.transient());
        using SizeT = decltype(map.size());

        map = {};

        SizeT size{};
        ar >> BOOST_SERIALIZATION_NVP(size);

        for (auto i = SizeT{}; i < size; ++i) {
            K k;
            T v;
            ar >> k >> v;
            map = std::move(map).set(std::move(k), std::move(v));
        }

        assert(size == map.size());
    }

    template<class Archive, class K, class T, class H, class E, class MP>
    inline void serialize(Archive &ar, immer::map<K, T, H, E, MP> &m, const unsigned int version)
    {
        boost::serialization::split_free(ar, m, version);
    }

}
