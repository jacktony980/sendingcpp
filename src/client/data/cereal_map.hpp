/*
 * Copyright (C) 2020 Tusooa Zhu
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
#include <cereal/cereal.hpp>
#include <immer/map.hpp>
namespace cereal
{

    template <typename Archive, typename K, typename T, typename H, typename E, typename MP>
    void CEREAL_SAVE_FUNCTION_NAME(Archive& ar, const immer::map<K, T, H, E, MP>& map)
    {
        ar(make_size_tag(static_cast<size_type>(map.size())));
        for (auto&& [k, v] : map)
            ar(k, v);
    }

    template <typename Archive, typename K, typename T, typename H, typename E, typename MP>
    void CEREAL_LOAD_FUNCTION_NAME(Archive& ar, immer::map<K, T, H, E, MP>& map)
    {
        size_type size;
        ar(make_size_tag(size));

        for (auto i = size_type{}; i < size; ++i) {
            K k;
            T v;
            ar(k, v);
            map = std::move(map).push_back(std::move(k), std::move(v));
        }

        assert(size == map.size());
    }
}
