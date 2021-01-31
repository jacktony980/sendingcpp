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

#include <boost/asio.hpp>

#include <promise-interface.hpp>

namespace Kazv
{
    template<class Exec, class T>
    class AsioPromise;

    namespace detail
    {
        template<class Exec>
        struct AsioPromiseHelper
        {
            template<class T>
            using PromiseType = AsioPromise<Exec, T>;
        };

        struct IdentityFunc
        {
            template<class T>
            constexpr T &&operator()(T &&t) const {
                return std::forward<T>(t);
            }
        };
    }

    template<class Exec, class T>
    class AsioPromise : public AbstractPromise<detail::AsioPromiseHelper<Exec>::template PromiseType, T>
    {
        using BaseT = AbstractPromise<detail::AsioPromiseHelper<Exec>::template PromiseType, T>;

        template<class FuncT, class PromiseT, class ResolveT>
        struct WaitHelper
        {
            void wait() {
                if (p.ready()) {
                    auto res = func(p.get());
                    using ResT = decltype(res);
                    if constexpr (isPromise<ResT>) {
                        boost::asio::post(
                            executor,
                            [w=WaitHelper<detail::IdentityFunc, ResT, ResolveT>{
                                    executor,
                                    res,
                                    detail::IdentityFunc{},
                                    resolve,
                                }]() mutable {
                                w.wait();
                            });
                    } else {
                        resolve(res);
                    }
                } else {
                    boost::asio::post(
                        executor,
                        [*this]() mutable {
                            wait();
                        }
                    );
                }
            }

            Exec executor;
            PromiseT p;
            FuncT func;
            ResolveT resolve;
        };
    public:
        AsioPromise(Exec executor, T value)
            : BaseT(this)
            , m_executor(std::move(executor)) {
            std::promise<T> p;
            m_val = p.get_future().share();
            p.set_value(std::move(value));
        }

        template<class Func>
        AsioPromise(Exec executor, Func &&callback)
            : BaseT(this)
            , m_executor(std::move(executor)) {
            auto p = std::make_shared<std::promise<T>>();
            m_val = p->get_future().share();
            auto resolve =
                [p=std::move(p)](T val) mutable {
                    p->set_value(std::move(val));
                };

            boost::asio::post(
                m_executor,
                [=, callback=std::forward<Func>(callback),
                 resolve=std::move(resolve),
                 guard=boost::asio::executor_work_guard(m_executor)]() {
                    callback(resolve);
                });
        }

        // FuncT: (DataT) -> AnotherDataT
        // where AnotherDataT = PromiseThenResult<FuncT, typename BaseT::DataT>
        template<class FuncT>
        auto then(FuncT &&func)
            -> AsioPromise<Exec,
                           PromiseThenResult<FuncT, typename BaseT::DataT>> {
            return AsioPromise<Exec,
                               PromiseThenResult<FuncT, typename BaseT::DataT>>(
                m_executor,
                [=, func=std::forward<FuncT>(func), *this](auto resolve) {
                    auto waitHelper = WaitHelper<std::decay_t<FuncT>,
                                                 AsioPromise,
                                                 std::decay_t<decltype(resolve)>>{
                        m_executor,
                        *this,
                        func,
                        resolve
                    };
                    waitHelper.wait();
                });
        }

        bool ready() const {
            return m_val.wait_for(std::chrono::seconds(0)) == std::future_status::ready;
        }

        T get() const {
            return m_val.get();
        }
    private:
        Exec m_executor;
        std::shared_future<T> m_val;
    };

    template<class Exec>
    class AsioPromiseHandler : public PromiseInterface<AsioPromiseHandler<Exec>,
                                                       detail::AsioPromiseHelper<Exec>::template PromiseType>
    {
        using BaseT = PromiseInterface<AsioPromiseHandler<Exec>,
                                       detail::AsioPromiseHelper<Exec>::template PromiseType>;
    public:
        template<class T>
        using PromiseT = AsioPromise<Exec, T>;

        AsioPromiseHandler(Exec executor)
            : BaseT(this)
            , m_executor(executor)
            {}

        AsioPromiseHandler(const AsioPromiseHandler &that)
            : BaseT(this)
            , m_executor(that.m_executor)
            {}

        AsioPromiseHandler(AsioPromiseHandler &&that)
            : BaseT(this)
            , m_executor(std::move(that.m_executor))
            {}

        AsioPromiseHandler &operator=(const AsioPromiseHandler &that) {
            m_executor = that.m_executor;
            return *this;
        }

        AsioPromiseHandler &operator=(AsioPromiseHandler &&that) {
            m_executor = std::move(that.m_executor);
            return *this;
        }

        template<class T, class FuncT>
        PromiseT<T> create(FuncT &&func) {
            return PromiseT<T>(m_executor, std::forward<FuncT>(func));
        }

        template<class T>
        PromiseT<T> createResolved(T val) {
            return PromiseT<T>(m_executor, std::move(val));
        }

    private:
        Exec m_executor;
    };

    template<class Exec>
    AsioPromiseHandler(Exec) -> AsioPromiseHandler<Exec>;
}
