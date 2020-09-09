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

#include "client/client.hpp"
#include "clientwrap.hpp"

namespace Kazv
{
    namespace detail
    {
        // emulates declval() but returns lvalue reference
        template<class T>
        typename std::add_lvalue_reference<T>::type declref() noexcept;
    }

    template<class EventLoop, class Xform, class ...Enhancers>
    class Sdk
    {
        using ClientT = ::Kazv::Client;
        using ActionT = typename ClientT::Action;

        using StoreT = decltype(
            lager::make_store<ActionT>(
                std::declval<ClientT>(),
                &ClientT::update,
                std::declval<EventLoop>(),
                lager::with_deps(
                    std::ref(detail::declref<JobInterface>()),
                    std::ref(detail::declref<EventInterface>())
                    ),
                std::declval<Enhancers>()...)
            );

        using ContextT = lager::context<Client::Action>;
    public:
        Sdk(ClientT client,
            JobInterface &jobHandler,
            EventInterface &eventEmitter,
            EventLoop &&eventLoop,
            Xform &&xform,
            Enhancers &&...enhancers)
            : m_store(lager::make_store<ActionT>(
                          std::move(client),
                          &ClientT::update,
                          std::forward<EventLoop>(eventLoop),
                          lager::with_deps(
                              std::ref(jobHandler),
                              std::ref(eventEmitter)),
                          std::forward<Enhancers>(enhancers)...))
            , m_client(m_store.xform(std::forward<Xform>(xform))) {}

        ContextT context() const {
            return m_store;
        }

        ClientWrap client() const {
            return {m_client, m_store};
        }

    private:
        StoreT m_store;
        lager::reader<ClientT> m_client;
    };

    template<class EventLoop, class Xform, class ...Enhancers>
    inline auto makeSdk(Client client,
                        JobInterface &jobHandler,
                        EventInterface &eventEmitter,
                        EventLoop &&eventLoop,
                        Xform &&xform,
                        Enhancers &&...enhancers)
        -> Sdk<EventLoop, Xform, Enhancers...>
    {
        return { std::move(client),
                 jobHandler,
                 eventEmitter,
                 std::forward<EventLoop>(eventLoop),
                 std::forward<Xform>(xform),
                 std::forward<Enhancers>(enhancers)... };
    }
}
