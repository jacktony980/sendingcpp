
#pragma once

#include <string>
#include <tuple>
#include <immer/map.hpp>
#include <boost/container_hash/hash.hpp>

namespace Kazv
{
    template<class K, class V, class List, class Func>
    immer::map<K, V> merge(immer::map<K, V> map, List list, Func keyOf)
    {
        for (auto v : list) {
            auto key = keyOf(v);
            map = std::move(map).set(key, v);
        }
        return map;
    }

    inline std::string keyOfPresence(Event e) {
        return e.sender();
    }

    inline std::string keyOfAccountData(Event e) {
        return e.type();
    }

    inline std::string keyOfTimeline(Event e) {
        return e.id();
    }

    struct KeyOfState {
        std::string type;
        std::string stateKey;
    };
    template<class Archive>
    void serialize(Archive &ar, KeyOfState &m)
    {
        ar(m.type, m.stateKey);
    }

    inline bool operator==(KeyOfState a, KeyOfState b)
    {
        return a.type == b.type && a.stateKey == b.stateKey;
    }

    inline KeyOfState keyOfState(Event e) {
        return {e.type(), e.stateKey()};
    }
}
namespace std
{
    template<> struct hash<Kazv::KeyOfState>
    {
        std::size_t operator()(const Kazv::KeyOfState & k) const noexcept {
            std::size_t seed = 0;
            boost::hash_combine(seed, k.type);
            boost::hash_combine(seed, k.stateKey);
            return seed;
        }
    };
}
