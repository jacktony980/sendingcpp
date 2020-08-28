
#pragma once

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
        JsonWrap() = default;
        JsonWrap(json&& j) : m_d(1, std::move(j)) {}
        JsonWrap(const json& j) : m_d(1, j) {}

        const json &get() const { return m_d.at(0); }
        operator json() const { return m_d.at(0); }
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
