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
    template<class Action, class Deps = lager::deps<>>
    class Context : public Deps
    {
    public:
        template<class Func>
        Context(Func &&dispatcher, BoolPromiseInterface ph, Deps deps)
            : Deps(std::move(deps))
            , m_ph(ph)
            , m_dispatcher(std::forward<Func>(dispatcher))
            {
            }

        template<class AnotherAction, class AnotherDeps,
                 std::enable_if_t<std::is_convertible_v<Action, AnotherAction>
                                  && std::is_convertible_v<AnotherDeps, Deps>, int> = 0>
        Context(const Context<AnotherAction, AnotherDeps> &that)
            : Deps(that)
            , m_ph(that.m_ph)
            , m_dispatcher(that.m_dispatcher)
            {
            }

        template<class AnotherAction, class AnotherDeps, class Conv,
                 std::enable_if_t<std::is_convertible_v<std::invoke_result_t<Conv, Action>, AnotherAction>
                                  && std::is_convertible_v<AnotherDeps, Deps>, int> = 0>
        Context(const Context<AnotherAction, AnotherDeps> &that, Conv &&conv)
            : Deps(that)
            , m_ph(that.m_ph)
            , m_dispatcher([=,
                            thatDispatcher=that.m_dispatcher,
                            conv=std::forward<Conv>(conv)](Action a) {
                               thatDispatcher(conv(std::move(a)));
                           })
            {
            }

        BoolPromise dispatch(Action a) const {
            return m_dispatcher(std::move(a));
        }

        decltype(auto) deps() {
            return static_cast<Deps &>(*this);
        }

        template<class Func>
        BoolPromise createPromise(Func func) const {
            return m_ph.create(std::move(func));
        }

        template<class Func>
        BoolPromise createWaitingPromise(Func func) const {
            return m_ph.createResolveToPromise(std::move(func));
        }

        BoolPromise createResolvedPromise(bool v) const {
            return m_ph.createResolved(v);
        }

        BoolPromiseInterface promiseInterface() const {
            return m_ph;
        }

    private:
        template <typename AnotherAction, typename AnotherDeps>
        friend class Context;
        BoolPromiseInterface m_ph;
        std::function<BoolPromise(Action)> m_dispatcher;
    };

    template<class Action, class Deps = lager::deps<>>
    class Effect
    {
    public:
        using ContextT = Context<Action, Deps>;

        template<class Func>
        Effect(Func func) {
            if constexpr (std::is_same_v<std::invoke_result_t<Func, ContextT>, void>) {
                m_d = [func=std::move(func)](const auto &ctx) {
                          return ctx.createPromise(
                              [ctx,
                               func](auto resolve) {
                                  func(ctx);
                                  resolve(true);
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

        BoolPromise operator()(const ContextT &ctx) const {
            return m_d(ctx);
        }

    private:
        std::function<BoolPromise(const ContextT &)> m_d;
    };

    template<class Reducer, class Model, class Action, class Deps>
    constexpr bool hasEffect = boost::hana::is_valid(
        []() -> decltype((void)Effect<Action, Deps>(
                             std::get<1>(std::declval<std::invoke_result_t<Reducer, Model, Action>>()))) {}
        )();

}
