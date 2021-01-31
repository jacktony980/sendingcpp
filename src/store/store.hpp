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

#include <lager/state.hpp>
#include <lager/deps.hpp>
#include <lager/util.hpp>
#include <lager/context.hpp>
#include <lager/store.hpp>
#include "context.hpp"

namespace Kazv
{
    template<class Action,
             class Model,
             class Reducer,
             class Deps = lager::deps<>,
             class Tag = lager::automatic_tag>
    class Store
    {
    public:
        using ContextT = Context<Action, Deps>;

        template<class PH>
        Store(Model initialModel, Reducer reducer, PH &&ph, Deps deps)
            : m_state(lager::make_state(std::move(initialModel), Tag{}))
            , m_reducer(reducer)
            , m_ph(std::forward<PH>(ph))
            , m_ctx([=](auto a) { return dispatch(std::move(a)); }, m_ph, deps)
            {}

        Store(const Store &that) = delete;
        Store &operator=(const Store &that) = delete;

        Store(Store &&that)
            : m_state(std::move(that.m_state))
            , m_reducer(std::move(that.m_reducer))
            , m_ph(std::move(that.m_ph))
            , m_ctx([=](auto a) { return dispatch(std::move(a)); }, m_ph, std::move(that.m_ctx.deps()))
            {}

        Store &operator=(Store &&that) {
            m_state = std::move(that.m_state);
            m_reducer = std::move(that.reducr);
            m_ph = std::move(that.m_ph);
            m_ctx = ContextT([=](auto a) { return dispatch(std::move(a)); }, m_ph, std::move(that.m_ctx.deps()));
            return *this;
        }

        auto dispatch(Action a) -> BoolPromise {
            return m_ph.createResolved(true)
                .then(
                    [=, a=std::move(a)](auto) {
                        if constexpr (hasEffect<Reducer, Model, Action, Deps>) {
                            auto [newModel, eff] = m_reducer(m_state.get(), a);
                            m_state.set(newModel);
                            return eff(m_ctx);
                        } else {
                            auto newModel = m_reducer(m_state.get(), a);
                            m_state.set(newModel);
                            return m_ph.createResolved(true);
                        }
                    });
        }

        operator lager::reader<Model>() const {
            return m_state;
        }

        template<class A2, class D2>
        operator Context<A2, D2>() const {
            return m_ctx;
        }

        ContextT context() const {
            return m_ctx;
        }

    private:
        lager::state<Model, Tag> m_state;
        Reducer m_reducer;
        BoolPromiseInterface m_ph;
        ContextT m_ctx;
    };

    template<class ActionTraits,
             class Tag,
             class Model,
             class Reducer,
             class PH,
             class Deps>
    auto makeStoreImpl(ActionTraits action, Model initialModel, Reducer reducer, PH &&ph, Deps &&deps)
    {
        using Action = typename ActionTraits::type;
        return Store<Action, Model, Reducer, Deps, Tag>(
            std::move(initialModel), std::move(reducer),
            std::forward<PH>(ph), std::forward<Deps>(deps));
    }

    template<class Action,
             class Tag = lager::automatic_tag,
             class Model,
             class Reducer,
             class PH,
             class ...Enhancers
             >
    auto makeStore(Model &&initialModel, Reducer &&reducer, PH &&ph, Enhancers &&...enhancers)
    {
        auto enhancer = zug::comp(std::forward<Enhancers>(enhancers)...);
        auto factory = enhancer(
            [&](auto actionTraits,
                auto &&model,
                auto &&reducer,
                auto &&ph,
                auto &&deps) {
                using ActionTraits = decltype(actionTraits);

                using ModelF = decltype(model);
                using ReducerF = decltype(reducer);
                using PHF = decltype(ph);
                using DepsF = decltype(deps);

                using ActionT = typename ActionTraits::type;
                using ModelT = std::decay_t<ModelF>;
                using ReducerT = std::decay_t<ReducerF>;
                using DepsT = std::decay_t<DepsF>;
                return Store<ActionT, ModelT, ReducerT, DepsT, Tag>(
                    std::forward<ModelF>(initialModel), std::forward<ReducerF>(reducer),
                    std::forward<PHF>(ph), std::forward<DepsF>(deps));
            });
        return factory(
            lager::type_<Action>{},
            std::forward<Model>(initialModel), std::forward<Reducer>(reducer),
            std::forward<PH>(ph), lager::deps<>{});
    }
}
