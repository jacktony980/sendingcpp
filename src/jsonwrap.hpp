
#pragma once

#ifndef NDEBUG
#include <lager/debug/cereal/struct.hpp>
#endif

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
#ifndef NDEBUG
        template <class Archive>
        void save( Archive & ar ) const {
            ar( get().dump() );
        }

        template <class Archive>
        void load( Archive & ar ) {
            json j;
            ar( j );
            m_d = immer::array<json>(1, std::move(j));
        }
#endif
    };
}

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
