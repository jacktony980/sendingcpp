
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
