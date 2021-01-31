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

#include <catch2/catch.hpp>

#include <asio-promise-handler.hpp>
#include <store.hpp>
#include <context.hpp>

using namespace Kazv;

struct BackInserter
{
    BackInserter(const BackInserter &) = delete;
    BackInserter(BackInserter &&) = default;

    void operator()(int a) const { insertFunc(a); }
    std::function<void(int)> insertFunc;
};

TEST_CASE("Store should behave properly", "[store]")
{
    boost::asio::io_context ioContext;
    auto ph = AsioPromiseHandler(ioContext.get_executor());

    std::vector<int> results;

    BackInserter bi = { [&results](int a) { results.push_back(a); } };

    using Model = int;
    using Action = int;
    using Deps = lager::deps<BackInserter &>;
    using Result = std::pair<Model, Effect<Action, Deps>>;
    using Reducer = std::function<Result(Model, Action)>;

    Reducer update = [&results](Model m, Action a) -> Result {
                         if (a > 0) {
                             return { m + a, lager::noop };
                         } else {
                             auto newM = m + a;
                             return { newM,
                                      [&results, newM](auto &&ctx) {
                                          auto &bi = lager::get<BackInserter &>(ctx);
                                          bi(newM);
                                          return ctx.dispatch(1);
                                      }
                             };
                         }
                     };

    auto store = makeStore<Action>(Model{}, update, ph, lager::with_deps(std::ref(bi)));

    lager::reader<Model> reader = store;

    Context<Action> ctx = store.context();

    store.dispatch(1)
        .then([=](auto res) {
                  REQUIRE(res);
                  REQUIRE(reader.get() == 1);
              })
        .then([=](auto) {
                  return ctx.dispatch(2);
              })
        .then([=](auto) {
                  REQUIRE(reader.get() == 3); // 1 + 2
              })
        .then([=](auto) {
                  return ctx.dispatch(-5);
              })
        .then([=](auto) {
                  REQUIRE(reader.get() == -1); // 3 - 5 + 1
              })
        .then([=, &ioContext](auto) {
                  return ctx.createWaitingPromise(
                      [=, &ioContext](auto resolve) {
                          auto timer = std::make_shared<boost::asio::steady_timer>(ioContext);
                          timer->expires_after(std::chrono::milliseconds(300));
                          timer->async_wait(
                              [ctx, timer, resolve](const boost::system::error_code& error) {
                                  if (! error) {
                                      resolve(ctx.createResolvedPromise(true)
                                              .then([=](auto) { return ctx.dispatch(10); }));
                                  }
                              });
                      });
              })
        .then([=](auto) {
                  REQUIRE(reader.get() == 9); // -1 + 10
              });

    ioContext.run();
    REQUIRE(results[0] == -2); // 3 - 5
}
