
#pragma once
#include <string>
#include <nlohmann/json.hpp>
#include <immer/array.hpp>

namespace Kazv
{
    using json = nlohmann::json;

    using Bytes = std::string;

    enum Status : bool
    {
        FAIL,
        SUCC,
    };

    class JsonWrap
    {
        // Cannot use box here, because it causes the resulting json
        // to be wrapped into an array.
        // https://github.com/arximboldi/immer/issues/155
        immer::array<json> m_d;
    public:
        JsonWrap(json&& j) : m_d(1, std::move(j)) {}
        JsonWrap(const json& j) : m_d(1, j) {}

        auto get() const { return m_d.at(0); }
    };

    using namespace std::string_literals;
}
