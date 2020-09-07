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

#include <lager/debug/cereal/struct.hpp>

#include <nlohmann/json.hpp>
#include <immer/array.hpp>

namespace Kazv
{
    using json = nlohmann::json;

    class JsonWrap
    {
        // Cannot use box here, because it causes the resulting json
        // to be wrapped into an array.
        // https://github.com/arximboldi/immer/issues/155
        immer::array<json> m_d;
    public:
        JsonWrap() : m_d(1, json()) {}
        JsonWrap(json&& j) : m_d(1, std::move(j)) {}
        JsonWrap(const json& j) : m_d(1, j) {}

        const json &get() const { return m_d.at(0); }
        operator json() const { return m_d.at(0); }

        template <class Archive>
        void save(Archive & ar, std::uint32_t const /*version*/) const {
            ar( get().dump() );
        }

        template <class Archive>
        void load(Archive & ar, std::uint32_t const /*version*/) {
            std::string j;
            ar( j );
            m_d = immer::array<json>(1, json::parse(std::move(j)));
        }

    };
}

CEREAL_CLASS_VERSION(Kazv::JsonWrap, 0);

namespace nlohmann
{
    template <>
    struct adl_serializer<Kazv::JsonWrap> {
        static void to_json(json& j, Kazv::JsonWrap w) {
            j = w.get();
        }

        static void from_json(const json& j, Kazv::JsonWrap &w) {
            w = Kazv::JsonWrap(j);
        }
    };

}

namespace Kazv
{
    inline bool operator==(JsonWrap a, JsonWrap b)
    {
        return a.get() == b.get();
    }
}
