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

#include <iterator>
#include <boost/hana/type.hpp>

#include <lager/reader.hpp>
#include <lager/lenses/optional.hpp>

#include <zug/into.hpp>
#include <zug/tuplify.hpp>
#include <zug/transducer/each.hpp>
#include <zug/run.hpp>

namespace Kazv
{
    namespace detail
    {
        inline auto hasPushBack = boost::hana::is_valid(
            [](auto t) -> decltype(
                (void)
                t.push_back(std::declval<typename decltype(t)::value_type>())) { });

        inline auto hasInsert = boost::hana::is_valid(
            [](auto t) -> decltype(
                (void)
                t.insert(std::declval<typename decltype(t)::value_type>())) { });

        struct ImmerPushBackOrInsertT
        {
            template<class Container, class ...InputRanges>
            auto operator()(Container container, InputRanges &&...ins) {
                if constexpr (hasPushBack(container)) {
                    return std::move(container)
                        .push_back(zug::tuplify(std::forward<InputRanges>(ins)...));
                } else {
                    static_assert(hasInsert(container),
                                  "Container must have either push_back() or insert() method");
                    return std::move(container)
                        .insert(zug::tuplify(std::forward<InputRanges>(ins)...));
                }
            }
        };

        inline ImmerPushBackOrInsertT immerPushBackOrInsert{};

        struct IntoImmerT
        {
            // We check for both transient_type and persistent_type
            // because for some containers, transient_type is not
            // implemented.
            inline static auto hasTransientType = boost::hana::is_valid(
                [](auto t) -> boost::hana::type<typename decltype(t)::transient_type::persistent_type> {});

            template<class Container, class Func, class ...InputRanges>
            auto operator()(Container &&m, Func &&transducer, InputRanges &&...ins) const {
                using ContainerT = std::decay_t<Container>;
                if constexpr (hasTransientType(m)) {
                    using TransientT = typename ContainerT::transient_type;
                    return zug::into(
                        std::forward<TransientT>(m.transient()),
                        std::forward<Func>(transducer),
                        std::forward<InputRanges>(ins)...)
                        .persistent();
                } else {
                    ContainerT c;
                    zug::run(
                        std::forward<Func>(transducer)
                        | zug::each(
                            [&](auto ...ins) {
                                c = immerPushBackOrInsert(std::move(c), std::move(ins)...);
                            }),
                        std::forward<InputRanges>(ins)...);
                    return c;
                }
            }

        };
    }
    inline detail::IntoImmerT intoImmer{};

    namespace detail
    {
        inline auto looksLikeImmer = boost::hana::is_valid(
            [](auto t) -> boost::hana::type<typename decltype(t)::transient_type> { });
        struct ContainerMapT
        {
            template<class ResultContainer, class Func>
            auto operator()(ResultContainer c, Func transducer) {
                return zug::map(
                    [=](auto m) {
                        if constexpr (looksLikeImmer(m)) {
                            return intoImmer(c, transducer, m);
                        } else {
                            return zug::into(c, transducer, m);
                        }
                    });
            }
        };
    }

    inline detail::ContainerMapT containerMap{};

    namespace CursorOp
    {
        template<class Cursor>
        inline auto operator+(Cursor &&c)
        {
            return (~std::forward<Cursor>(c)).get();
        }

        template<class Cursor>
        inline auto operator~(Cursor &&c)
        {
            return std::forward<Cursor>(c).make();
        }
    }

    inline constexpr auto eventContent =
        zug::map([](auto &&event) {
                     return std::forward<decltype(event)>(event).content();
                 });

    namespace detail
    {
        inline auto isJsonWrap = boost::hana::is_valid(
            [](auto t) -> decltype((void)t.get()) { });
    }

    template<class T, class V>
    constexpr auto jsonAtOr(T &&key, V &&defaultValue)
    {
        return
            zug::map([key=std::forward<T>(key), def=std::forward<V>(defaultValue)](auto &&j) -> V {
                         using JsonT = decltype(j);
                         if constexpr (isJsonWrap(j)) {
                             return j.get().contains(key)
                                 ? V(std::forward<JsonT>(j).get().at(std::forward<T>(key)))
                                 : std::forward<V>(def);
                         } else {
                             return j.contains(key)
                                 ? V(std::forward<JsonT>(j).at(std::forward<T>(key)))
                                 : std::forward<V>(def);
                         }
                     });
    }
}

#define KAZV_WRAP_ATTR(_type, _d, _attr)          \
    inline auto _attr() const {                   \
        return (_d)[&_type::_attr];               \
    }
