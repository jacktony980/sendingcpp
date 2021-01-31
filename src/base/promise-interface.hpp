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

#pragma once

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

    class BoolPromise : TypelessPromise
    {
    public:
        using DataT = bool;

        template<class DeriveT>
        BoolPromise(DeriveT obj)
            : m_d(std::unique_ptr<Concept>(new Model<DeriveT>(std::move(obj)))) {}

        BoolPromise(const BoolPromise &that)
            : m_d(that.m_d->clone())
            {}

        BoolPromise(BoolPromise &&that)
            : m_d(std::move(that.m_d))
            {}

        BoolPromise &operator=(const BoolPromise &that) {
            m_d = that.m_d->clone();
            return *this;
        }

        BoolPromise &operator=(BoolPromise &&that) {
            m_d = std::move(that.m_d);
            return *this;
        }

        template<class F>
        BoolPromise then(F &&f) {
            if constexpr (std::is_same_v<std::invoke_result_t<F, bool>, void>) {
                return m_d->thenVoid(f);
            } else if constexpr (std::is_same_v<std::invoke_result_t<F, bool>, bool>) {
                return m_d->thenBool(f);
            } else {
                return m_d->thenPromise(f);
            }
        }

        bool ready() const { return m_d->ready(); }
        bool get() const { return m_d->get(); }

    private:
        struct Concept
        {
            virtual ~Concept() = default;
            virtual BoolPromise thenVoid(std::function<void(bool)> f) = 0;
            virtual BoolPromise thenBool(std::function<bool(bool)> f) = 0;
            virtual BoolPromise thenPromise(std::function<BoolPromise(bool)> f) = 0;
            virtual std::unique_ptr<Concept> clone() const = 0;
            virtual bool ready() const = 0;
            virtual bool get() const = 0;
        };

        template<class DeriveT>
        struct Model : public Concept
        {
            Model(DeriveT obj) : instance(std::move(obj)) {}
            ~Model() override = default;
            BoolPromise thenVoid(std::function<void(bool)> f) override {
                return instance.then([=](bool v) { f(v); return true; });
            }
            BoolPromise thenBool(std::function<bool(bool)> f) override {
                return instance.then([=](bool v) { return f(v); });
            }
            BoolPromise thenPromise(std::function<BoolPromise(bool)> f) override {
                return instance.then([=](bool v) { return f(v); });
            }
            std::unique_ptr<Concept> clone() const override {
                return std::unique_ptr<Concept>(new Model<DeriveT>(instance));
            }
            bool ready() const override {
                return instance.ready();
            }
            bool get() const override {
                return instance.get();
            }
            DeriveT instance;
        };

        std::unique_ptr<Concept> m_d;
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

    class BoolPromiseInterface
    {
    public:
        using ResolveT = std::function<void(bool)>;
        using ResolveToPromiseT = std::function<void(BoolPromise)>;

        template<class DeriveT>
        BoolPromiseInterface(DeriveT obj)
            : m_d(std::unique_ptr<Concept>(new Model<std::decay_t<DeriveT>>(std::move(obj)))) {
            if (! m_d) {
                throw std::logic_error("promise handler is empty");
            }
        }

        BoolPromiseInterface(const BoolPromiseInterface &that)
            : m_d(that.m_d) {
            if (! m_d) {
                throw std::logic_error("promise handler is empty");
            }
        }
        BoolPromiseInterface(BoolPromiseInterface &&that)
            : m_d(std::move(that.m_d)) {
            if (! m_d) {
                throw std::logic_error("promise handler is empty");
            }
        }

        BoolPromiseInterface &operator=(const BoolPromiseInterface &that) {
            m_d = that.m_d;
            return *this;
        }

        BoolPromiseInterface &operator=(BoolPromiseInterface &&that) {
            m_d = std::move(that.m_d);
            return *this;
        }

        BoolPromise create(std::function<void(ResolveT)> f) const {
            return m_d->create(f);
        }

        BoolPromise createResolveToPromise(std::function<void(ResolveToPromiseT)> f) const {
            return m_d->createResolveToPromise(f);
        }

        BoolPromise createResolved(bool v) const {
            return m_d->createResolved(v);
        }

        template<class RangeT>
        BoolPromise all(RangeT promises) const {
            if (promises.empty()) {
                return createResolved(true);
            }
            auto p1 = *(promises.begin());
            promises.erase(promises.begin());
            return p1.then([*this, promises=std::move(promises)](bool val) mutable {
                               if (! val) {
                                   return createResolved(false);
                               }
                               return all(std::move(promises));
                           });
        }

    private:
        struct Concept
        {
            virtual ~Concept() = default;
            virtual BoolPromise create(std::function<void(ResolveT)> f) = 0;
            virtual BoolPromise createResolveToPromise(std::function<void(ResolveToPromiseT)> f) = 0;
            virtual BoolPromise createResolved(bool v) = 0;
        };

        template<class DeriveT>
        struct Model : public Concept
        {
            static_assert(std::is_same_v<std::decay_t<DeriveT>, DeriveT>, "DeriveT must not be a reference");
            Model(DeriveT obj) : instance(std::move(obj)) {}
            ~Model() override = default;
            BoolPromise create(std::function<void(ResolveT)> f) override {
                return instance.template create<bool>(f);
            }
            BoolPromise createResolveToPromise(std::function<void(ResolveToPromiseT)> f) override {
                return instance.template create<bool>(f);
            }
            BoolPromise createResolved(bool v) override {
                return instance.createResolved(v);
            }
            DeriveT instance;
        };

        std::shared_ptr<Concept> m_d;
    };
}
