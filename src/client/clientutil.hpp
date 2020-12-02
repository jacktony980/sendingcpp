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

#include <string>
#include <tuple>
#include <immer/map.hpp>
#include <lager/deps.hpp>
#include <boost/container_hash/hash.hpp>

#include <jobinterface.hpp>
#include <eventinterface.hpp>

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

    inline std::string keyOfEphemeral(Event e) {
        return e.type();
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

    template<class Context>
    JobInterface &getJobHandler(Context &&ctx)
    {
        return lager::get<JobInterface &>(std::forward<Context>(ctx));
    }

    template<class Context>
    EventInterface &getEventEmitter(Context &&ctx)
    {
        return lager::get<EventInterface &>(std::forward<Context>(ctx));
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
