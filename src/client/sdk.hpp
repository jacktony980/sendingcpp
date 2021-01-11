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
#include <lager/store.hpp>

#include "sdk-model.hpp"
#include "client.hpp"

namespace Kazv
{
    template<class EventLoop, class Xform, class ...Enhancers>
    class Sdk
    {
        using ModelT = ::Kazv::SdkModel;
        using ClientT = ::Kazv::ClientModel;
        using ActionT = typename ModelT::Action;

        using StoreT = decltype(
            lager::make_store<ActionT>(
                std::declval<ModelT>(),
                &ModelT::update,
                std::declval<EventLoop>(),
                lager::with_deps(
                    std::ref(detail::declref<JobInterface>()),
                    std::ref(detail::declref<EventInterface>())
                    ),
                std::declval<Enhancers>()...)
            );

        using ContextT = lager::context<ActionT>;
    public:
        Sdk(ModelT model,
            JobInterface &jobHandler,
            EventInterface &eventEmitter,
            EventLoop &&eventLoop,
            Xform &&xform,
            Enhancers &&...enhancers)
            : m_store(lager::make_store<ActionT>(
                          std::move(model),
                          &ModelT::update,
                          std::forward<EventLoop>(eventLoop),
                          lager::with_deps(
                              std::ref(jobHandler),
                              std::ref(eventEmitter)),
                          std::forward<Enhancers>(enhancers)...))
            , m_sdk(m_store.xform(std::forward<Xform>(xform)))
            , m_client(m_sdk[&ModelT::client]) {}

        ContextT context() const {
            return m_store;
        }

        Client client() const {
            return {m_client, m_store};
        }

    private:
        StoreT m_store;
        lager::reader<ModelT> m_sdk;
        lager::reader<ClientT> m_client;
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
}
