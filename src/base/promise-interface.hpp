/*
 * Copyright (C) 2021 Tusooa Zhu <tusooa@vista.aero>
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

#include <memory>
#include <type_traits>
#include <functional>

namespace Kazv
{
    class TypelessPromise
    {
    };

    template<template<class> class DeriveT, class T>
    class AbstractPromise;

    namespace detail
    {
        template<class T, class = void>
        struct PromiseParameterT
        {
            using type = T;
        };

        template<class PromiseT>
        struct PromiseParameterT<PromiseT,
                                 std::void_t<std::enable_if_t<std::is_base_of_v<TypelessPromise, PromiseT>, int>>>
        {
            using type = typename PromiseT::DataT;
        };

        template<class T>
        using PromiseParameter = typename PromiseParameterT<T>::type;

        template<class T, class = void>
        struct PromiseResult
        {
        };

        template<class T>
        struct PromiseResult<T, std::void_t<std::invoke_result_t<T>>>
        {
            using Res = std::invoke_result_t<T>;

            using type = PromiseParameter<Res>;
        };

        template<class T>
        using PromiseResultT = typename PromiseResult<T>::type;


        template<class T, class P>
        struct PromiseThenResultT
        {
            using Res = std::invoke_result_t<T, PromiseParameter<P>>;

            using type = PromiseParameter<Res>;
        };
    }

    template<class T, class P>
    using PromiseThenResult = typename detail::PromiseThenResultT<T, P>::type;

    template<class T>
    constexpr auto isPromise = std::is_base_of_v<TypelessPromise, T>;

    template<template<class> class DeriveT, class T>
    class AbstractPromise : TypelessPromise
    {
    public:
        using DataT = T;

        static_assert(!isPromise<DataT>, "Cannot create a promise of promise");

        AbstractPromise(DeriveT<T> *obj) : m_derived(obj) {}

        template<class FuncT>
        auto then(FuncT &&func)
            -> DeriveT<PromiseThenResult<FuncT, DataT>> {
            return m_derived->then(std::forward<FuncT>(func));
        }

    private:
        DeriveT<T> *m_derived;
    };

    template<class DeriveT, template<class> class PromiseT>
    class PromiseInterface
    {
    public:
        PromiseInterface(DeriveT *obj) : m_derived(obj) {}

        template<class T, class FuncT>
        auto create(FuncT &&func) -> PromiseT<T>;

        template<class T>
        auto createResolved(T &&val) -> PromiseT<T>;

    private:
        DeriveT *m_derived;
    };

    template<class DeriveT, template<class> class PromiseT>
    template<class T, class FuncT>
    auto PromiseInterface<DeriveT, PromiseT>::create(FuncT &&func) -> PromiseT<T> {
        return m_derived->create(std::forward<FuncT>(func));
    }

    template<class DeriveT, template<class> class PromiseT>
    template<class T>
    auto PromiseInterface<DeriveT, PromiseT>::createResolved(T &&val) -> PromiseT<T> {
        return m_derived->createResolved(std::forward<T>(val));
    }
}
