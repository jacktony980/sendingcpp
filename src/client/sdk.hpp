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
#include <libkazv-config.hpp>
#include <lager/store.hpp>

#include <store.hpp>

#include "sdk-model.hpp"
#include "sdk-model-cursor-tag.hpp"
#include "client.hpp"
#include "thread-safety-helper.hpp"

namespace Kazv
{
    /**
     * Contain the single source of truth of a matrix sdk.
     */
    template<class EventLoop, class Xform, class ...Enhancers>
    class Sdk
    {
        using ModelT = ::Kazv::SdkModel;
        using ClientT = ::Kazv::ClientModel;
        using ActionT = typename ModelT::Action;
        using CursorT = lager::reader<ModelT>;
        using CursorTSP = std::shared_ptr<const CursorT>;

        using StoreT = decltype(
            makeStore<ActionT>(
                std::declval<ModelT>(),
                &ModelT::update,
                std::declval<EventLoop>(),
                lager::with_deps(
                    std::ref(detail::declref<JobInterface>()),
                    std::ref(detail::declref<EventInterface>()),
                    lager::dep::as<SdkModelCursorKey>(std::declval<std::function<CursorTSP()>>())
#ifdef KAZV_USE_THREAD_SAFETY_HELPER
                    , std::ref(detail::declref<EventLoopThreadIdKeeper>())
#endif
                    ),
                std::declval<Enhancers>()...)
            );

        using DepsT = lager::deps<JobInterface &, EventInterface &, SdkModelCursorKey
#ifdef KAZV_USE_THREAD_SAFETY_HELPER
                                  , EventLoopThreadIdKeeper &
#endif
                                  >;

        using ContextT = Context<ActionT, DepsT>;
    public:
        Sdk(ModelT model,
            JobInterface &jobHandler,
            EventInterface &eventEmitter,
            EventLoop &&eventLoop,
            Xform &&xform,
            Enhancers &&...enhancers)
            : m_d(std::make_unique<Private>(
                      std::move(model), jobHandler, eventEmitter,
                      std::forward<EventLoop>(eventLoop), std::forward<Xform>(xform),
                      std::forward<Enhancers>(enhancers)...)) {}

        /**
         * Get the context associated with this.
         *
         * The returned context is thread-safe if every thread calls with
         * different instances.
         */
        ContextT context() const {
            return m_d->store;
        }

        /**
         * Get a Client representing this.
         *
         * The returned Client belongs to the thread where the promise handler runs.
         */
        Client client() const {
            return {Client::InEventLoopTag{}, ContextT(m_d->store)};
        }

    private:
        struct Private
        {
            Private(ModelT model,
                    JobInterface &jobHandler,
                    EventInterface &eventEmitter,
                    EventLoop &&eventLoop,
                    Xform &&xform,
                    Enhancers &&...enhancers)
                : store(makeStore<ActionT>(
                            std::move(model),
                            &ModelT::update,
                            std::forward<EventLoop>(eventLoop),
                            lager::with_deps(
                                std::ref(jobHandler),
                                std::ref(eventEmitter),
                                lager::dep::as<SdkModelCursorKey>(
                                    std::function<CursorTSP()>([this] { return sdk; }))
#ifdef KAZV_USE_THREAD_SAFETY_HELPER
                                , std::ref(keeper)
#endif
                                ),
                            std::forward<Enhancers>(enhancers)...))
                , sdk(std::make_shared<lager::reader<ModelT>>(store.reader().xform(std::forward<Xform>(xform))))
                {
#ifdef KAZV_USE_THREAD_SAFETY_HELPER
                    store.context().createResolvedPromise(EffectStatus{})
                        .then([this](auto &&) {
                                  keeper.set(std::this_thread::get_id());
                              });
#endif
                }
#ifdef KAZV_USE_THREAD_SAFETY_HELPER
            EventLoopThreadIdKeeper keeper;
#endif
            StoreT store;
            std::shared_ptr<const lager::reader<ModelT>> sdk;
        };

        std::unique_ptr<Private> m_d;
    };

    template<class EventLoop, class Xform, class ...Enhancers>
    inline auto makeSdk(SdkModel sdk,
                        JobInterface &jobHandler,
                        EventInterface &eventEmitter,
                        EventLoop &&eventLoop,
                        Xform &&xform,
                        Enhancers &&...enhancers)
        -> Sdk<EventLoop, Xform, Enhancers...>
    {
        return { std::move(sdk),
                 jobHandler,
                 eventEmitter,
                 std::forward<EventLoop>(eventLoop),
                 std::forward<Xform>(xform),
                 std::forward<Enhancers>(enhancers)... };
    }

    template<class EventLoop, class Xform, class ...Enhancers>
    inline auto makeDefaultEncryptedSdk(
        JobInterface &jobHandler,
        EventInterface &eventEmitter,
        EventLoop &&eventLoop,
        Xform &&xform,
        Enhancers &&...enhancers)
        -> Sdk<EventLoop, Xform, Enhancers...>
    {
        auto m = SdkModel{};
        m.client.crypto = Crypto();

        return makeSdk(std::move(m),
                       jobHandler,
                       eventEmitter,
                       std::forward<EventLoop>(eventLoop),
                       std::forward<Xform>(xform),
                       std::forward<Enhancers>(enhancers)...);
    }
}
