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
            : m_d{std::make_shared<Private>(
                lager::make_state(std::move(initialModel), Tag{}),
                reducer,
                BoolPromiseInterface{std::forward<PH>(ph)},
                deps)}
            {}

        Store(const Store &that) = delete;
        Store &operator=(const Store &that) = delete;

        Store(Store &&that)
            : m_d(std::move(that.m_d))
            {}

        Store &operator=(Store &&that) {
            m_d = std::move(that.m_d);
            return *this;
        }

        auto dispatch(Action a) -> BoolPromise {
            return m_d->dispatch(std::move(a));
        };

        operator lager::reader<Model>() const {
            return m_d->state;
        }

        lager::reader<Model> reader() const {
            return m_d->state;
        }

        template<class A2, class D2>
        operator Context<A2, D2>() const {
            return m_d->ctx;
        }

        ContextT context() const {
            return m_d->ctx;
        }

    private:
        struct Private
        {
            Private(lager::state<Model, Tag> state,
                    Reducer reducer,
                    BoolPromiseInterface ph,
                    Deps deps)
                : state(std::move(state))
                , reducer(std::move(reducer))
                , ph(std::move(ph))
                  // this Private will remain untouched between moves of Stores
                  // so it is safe to capture by referenced
                , ctx([this](auto a) { return dispatch(std::move(a)); }, this->ph, deps)
                {}
            ~Private() = default;
            lager::state<Model, Tag> state;
            Reducer reducer;
            BoolPromiseInterface ph;
            ContextT ctx;

            auto dispatch(Action a) -> BoolPromise {
                return ph.createResolved(true)
                    .then(
                        [this, a=std::move(a)](auto) {
                            if constexpr (hasEffect<Reducer, Model, Action, Deps>) {
                            auto [newModel, eff] = reducer(state.get(), a);
                            state.set(newModel);
                            return eff(ctx);
                        } else {
                            auto newModel = reducer(state.get(), a);
                            state.set(newModel);
                            return ph.createResolved(true);
                        }
                    });
        }
        };
        std::shared_ptr<Private> m_d;
    };

    namespace detail
    {
        constexpr auto compose()
        {
            return zug::identity;
        }

        template<class F1, class ...Fs>
        constexpr auto compose(F1 &&f1, Fs &&...fs)
        {
            return zug::comp(std::forward<F1>(f1), std::forward<Fs>(fs)...);
        }
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
        auto enhancer = detail::compose(std::forward<Enhancers>(enhancers)...);
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
