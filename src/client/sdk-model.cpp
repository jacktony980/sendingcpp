/*
 * Copyright (C) 2020 Tusooa Zhu <tusooa@vista.aero>
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

#include <libkazv-config.hpp>

#include <zug/into_vector.hpp>

#include "sdk-model.hpp"

#include <debug.hpp>

#include "actions/encryption.hpp"
#include "actions/states.hpp"

namespace Kazv
{

    static const int syncInterval = 2000; // ms

    SdkResult SdkModel::update(SdkModel s, SdkAction a)
    {
        return lager::match(a)(
            [&](ClientAction a) -> SdkResult {
                auto [newClient, clientEff] =
                    ClientModel::update(std::move(s.client), a);
                s.client = std::move(newClient);

                bool hasCrypto{s.client.crypto};

                auto jobs = s.client.popAllJobs();
                auto triggers = s.client.popAllTriggers();

                auto eff =
                    [=](auto &&ctx) {
                        auto sdkEffect = SdkEffect(clientEff);
                        auto effectPromise = sdkEffect(ctx);

                        auto &jh = getJobHandler(ctx);
                        auto &ee = getEventEmitter(ctx);

                        auto ph = ctx.promiseInterface();

                        auto promises = zug::into_vector(
                            zug::map([=, &jh](auto j) {
                                         return ctx.createWaitingPromise(
                                             [=, &jh](auto resolve) {
                                                 jh.submit(
                                                     j,
                                                     [=](Response r) {
                                                         resolve(ctx.dispatch(ProcessResponseAction{r}));
                                                     });
                                             });
                                     }), jobs);

                        auto combinedPromise = promises.size() ? ph.all(promises) : effectPromise;

                        for (auto t : triggers) {
                            ee.emit(t);
                        }
                        return combinedPromise;
                    };

                return { std::move(s), eff };
            }
            );
    }
}
