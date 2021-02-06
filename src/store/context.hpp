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

#include <lager/deps.hpp>

#include <boost/hana.hpp>

#include <promise-interface.hpp>

namespace Kazv
{
    template<class T, class Action, class Deps = lager::deps<>>
    class ContextBase : public Deps
    {
    public:
        using RetType = T;
        using PromiseInterfaceT = SingleTypePromiseInterface<RetType>;
        using PromiseT = SingleTypePromise<RetType>;
        template<class Func>
        ContextBase(Func &&dispatcher, PromiseInterfaceT ph, Deps deps)
            : Deps(std::move(deps))
            , m_ph(ph)
            , m_dispatcher(std::forward<Func>(dispatcher))
            {
            }

        template<class AnotherAction, class AnotherDeps,
                 std::enable_if_t<std::is_convertible_v<Action, AnotherAction>
                                  && std::is_convertible_v<AnotherDeps, Deps>, int> = 0>
        ContextBase(const ContextBase<RetType, AnotherAction, AnotherDeps> &that)
            : Deps(that)
            , m_ph(that.m_ph)
            , m_dispatcher(that.m_dispatcher)
            {
            }

        template<class AnotherAction, class AnotherDeps, class Conv,
                 std::enable_if_t<std::is_convertible_v<std::invoke_result_t<Conv, Action>, AnotherAction>
                                  && std::is_convertible_v<AnotherDeps, Deps>, int> = 0>
        ContextBase(const ContextBase<RetType, AnotherAction, AnotherDeps> &that, Conv &&conv)
            : Deps(that)
            , m_ph(that.m_ph)
            , m_dispatcher([=,
                            thatDispatcher=that.m_dispatcher,
                            conv=std::forward<Conv>(conv)](Action a) {
                               thatDispatcher(conv(std::move(a)));
                           })
            {
            }

        PromiseT dispatch(Action a) const {
            return m_dispatcher(std::move(a));
        }

        decltype(auto) deps() {
            return static_cast<Deps &>(*this);
        }

        template<class Func>
        PromiseT createPromise(Func func) const {
            return m_ph.create(std::move(func));
        }

        template<class Func>
        PromiseT createWaitingPromise(Func func) const {
            return m_ph.createResolveToPromise(std::move(func));
        }

        PromiseT createResolvedPromise(bool v) const {
            return m_ph.createResolved(v);
        }

        PromiseInterfaceT promiseInterface() const {
            return m_ph;
        }

    private:
        template<class R2, class A2, typename D2>
        friend class ContextBase;
        PromiseInterfaceT m_ph;
        std::function<PromiseT(Action)> m_dispatcher;
    };

    template<class A, class D = lager::deps<>>
    using Context = ContextBase<bool, A, D>;

    template<class T, class Action, class Deps = lager::deps<>>
    class EffectBase
    {
    public:
        using RetType = T;
        using PromiseT = SingleTypePromise<RetType>;
        using ContextT = ContextBase<RetType, Action, Deps>;

        template<class Func>
        EffectBase(Func func) {
            if constexpr (std::is_same_v<std::invoke_result_t<Func, ContextT>, void>) {
                m_d = [func=std::move(func)](const auto &ctx) {
                          return ctx.createPromise(
                              [ctx,
                               func](auto resolve) {
                                  func(ctx);
                                  resolve(PromiseCombination::defaultForPromiseThen(RetType()));
                              });
                      };
            } else {
                m_d = [func=std::move(func)](const auto &ctx) {
                          return ctx.createResolvedPromise(true)
                              .then([ctx,
                                     func](auto) {
                                        return func(ctx);
                                    });
                      };
            }
        }

        PromiseT operator()(const ContextT &ctx) const {
            return m_d(ctx);
        }

    private:
        std::function<PromiseT(const ContextT &)> m_d;
    };

    template<class A, class D = lager::deps<>>
    using Effect = EffectBase<bool, A, D>;

    template<class Reducer, class Model, class Action, class Deps>
    constexpr bool hasEffect = boost::hana::is_valid(
        []() -> decltype((void)Effect<Action, Deps>(
                             std::get<1>(std::declval<std::invoke_result_t<Reducer, Model, Action>>()))) {}
        )();

}
