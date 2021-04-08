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

#include <optional>

namespace boost::serialization
{
    template<class Archive, class T>
    void save(Archive &ar, const std::optional<T> &o, const unsigned int /* version */)
    {
        auto hasValue = o.has_value();
        ar << BOOST_SERIALIZATION_NVP(hasValue);

        if (hasValue) {
            const auto &value = o.value();
            ar << BOOST_SERIALIZATION_NVP(value);
        }
    }

    template<class Archive, class T>
    void load(Archive &ar, std::optional<T> &o, const unsigned int /* version */)
    {
        auto hasValue = bool{};
        ar >> BOOST_SERIALIZATION_NVP(hasValue);

        o.reset();

        if (hasValue) {
            T value;
            ar >> BOOST_SERIALIZATION_NVP(value);
            o = std::move(value);
        }
    }

    template<class Archive,
             class T>
    inline void serialize(Archive &ar, std::optional<T> &o, const unsigned int version)
    {
        boost::serialization::split_free(ar, o, version);
    }
}
