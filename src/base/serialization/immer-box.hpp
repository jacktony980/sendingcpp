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

#include <immer/box.hpp>

namespace boost::serialization
{

    template <class Archive, class T, class MP>
    void save(Archive &ar, const immer::box<T, MP> &box, const unsigned int /* version */)
    {
        ar << box.get();
    }

    template <class Archive, class T, class MP>
    void load(Archive &ar, immer::box<T, MP> &box, const unsigned int /* version */)
    {
        T val;
        ar >> val;

        box = val;
    }

    template<class Archive, class T, class MP>
    inline void serialize(Archive &ar, immer::box<T, MP> &box, const unsigned int version)
    {
        boost::serialization::split_free(ar, box, version);
    }

}
