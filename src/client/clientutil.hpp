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

#include <string>
#include <tuple>
#include <immer/map.hpp>
#include <zug/transducer/filter.hpp>
#include <zug/transducer/eager.hpp>
#include <lager/deps.hpp>
#include <boost/container_hash/hash.hpp>

#include <cursorutil.hpp>
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

    namespace
    {
        template<class ImmerT>
        struct ImmerIterator
        {
            using value_type = typename ImmerT::value_type;
            using reference = typename ImmerT::reference;
            using pointer = const value_type *;
            using difference_type = long int;
            using iterator_category = std::random_access_iterator_tag;

            ImmerIterator(const ImmerT &container, std::size_t index)
                : m_container(std::ref(container))
                , m_index(index)
                {}

            ImmerIterator &operator+=(difference_type d) {
                m_index += d;
                return *this;
            }

            ImmerIterator &operator-=(difference_type d) {
                m_index -= d;
                return *this;
            }

            difference_type operator-(ImmerIterator b) const {
                return index() - b.index();
            }

            ImmerIterator &operator++() {
                return *this += 1;
            }

            ImmerIterator operator++(int) {
                auto tmp = *this;
                *this += 1;
                return tmp;
            }

            ImmerIterator &operator--() {
                return *this -= 1;
            }

            ImmerIterator operator--(int) {
                auto tmp = *this;
                *this -= 1;
                return tmp;
            }


            reference &operator*() const {
                return m_container.get().at(m_index);
            }

            reference operator[](difference_type d) const;

            std::size_t index() const { return m_index; }

        private:

            std::reference_wrapper<const ImmerT> m_container;
            std::size_t m_index;
        };

        template<class ImmerT>
        auto ImmerIterator<ImmerT>::operator[](difference_type d) const -> reference
        {
            return *(*this + d);
        }

        template<class ImmerT>
        auto operator+(ImmerIterator<ImmerT> a, long int d)
        {
            return a += d;
        };

        template<class ImmerT>
        auto operator+(long int d, ImmerIterator<ImmerT> a)
        {
            return a += d;
        };

        template<class ImmerT>
        auto operator-(ImmerIterator<ImmerT> a, long int d)
        {
            return a -= d;
        };

        template<class ImmerT>
        auto immerBegin(const ImmerT &c)
        {
            return ImmerIterator<ImmerT>(c, 0);
        }

        template<class ImmerT>
        auto immerEnd(const ImmerT &c)
        {
            return ImmerIterator<ImmerT>(c, c.size());
        }
    }

    template<class ImmerT1, class RangeT2, class Pred, class Func>
    ImmerT1 sortedUniqueMerge(ImmerT1 base, RangeT2 addon, Pred exists, Func keyOf)
    {
        auto needToAdd = intoImmer(ImmerT1{},
                                   zug::filter([=](auto a) {
                                                   return !exists(a);
                                               }),
                                   addon);

        auto cmp = [=](auto a, auto b) {
                       return keyOf(a) < keyOf(b);
                   };

        for (auto item : needToAdd) {
            auto it = std::upper_bound(immerBegin(base), immerEnd(base), item, cmp);
            auto index = it.index();
            base = std::move(base).insert(index, item);
        }

        return base;
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
