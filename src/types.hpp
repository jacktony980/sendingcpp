
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

    template<class T,
             std::enable_if_t<std::is_same_v<decltype(std::declval<T>().empty()), bool>,
                                    int> = 0>
    inline void addToJsonIfNeeded(json &j, std::string name, T &&arg)
    {
        if (! arg.empty()) {
            j[name] = std::forward<T>(arg);
        }
    };

    template<class T>
    inline void addToJsonIfNeeded(json &j, std::string name, T &&arg)
    {
        j[name] = std::forward<T>(arg);
    };

    template<class T>
    inline void addToJsonIfNeeded(json &j, std::string name, std::optional<T> &&arg)
    {
        if (arg.has_value()) {
            j[name] = std::forward<T>(arg).value();
        }
    };

    // Provide a non-destructive way to add the map
    // to json.
    template<class MapT>
    inline void addPropertyMapToJson(json &j, MapT &&arg)
    {
        for (auto kv : std::forward<MapT>(arg)) {
            auto [k, v] = kv;
            j[k] = v;
        }
    };

    using namespace std::string_literals;
}
