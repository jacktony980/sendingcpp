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

#include "sdk-model.hpp"

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
                    [=, clientEff=std::move(clientEff)](auto &&ctx) {
                        clientEff(ctx);

                        auto &jh = getJobHandler(ctx);
                        auto &ee = getEventEmitter(ctx);

                        for (auto j : jobs) {
                            jh.submit(j,
                                      [=](Response r) {
                                          ctx.dispatch(ProcessResponseAction{r});
                                      });
                        }

                        for (auto t : triggers) {
                            ee.emit(t);
                            // start a sync (from posting filters) immediately after login successful
                            // apparently, the sync is guaranteed to be processed
                            // after this action has been processed
                            if (std::holds_alternative<LoginSuccessful>(t)) {
                                ctx.dispatch(PostInitialFiltersAction{});
                            }
                            // start a sync `syncInterval` ms after another sync
                            // if we use encryption, also upload one-time keys needed
                            else if (std::holds_alternative<SyncSuccessful>(t)) {
                                auto syncSuccessful = std::get<SyncSuccessful>(t);
                                if (hasCrypto) {
                                    ctx.dispatch(GenerateAndUploadOneTimeKeysAction{});
                                    ctx.dispatch(QueryKeysAction{syncSuccessful.isInitialSync});
                                }
                                jh.setTimeout(
                                    [=]() {
                                        ctx.dispatch(SyncAction{});
                                    }, syncInterval);
                            }
                            // start a sync or publish identity keys after posting initial filters
                            else if (std::holds_alternative<PostInitialFiltersSuccessful>(t)) {
                                if (hasCrypto) {
                                    ctx.dispatch(UploadIdentityKeysAction{});
                                } else {
                                    ctx.dispatch(SyncAction{});
                                }
                            }
                            // start sync after publishing identity keys
                            else if (std::holds_alternative<UploadIdentityKeysSuccessful>(t)) {
                                ctx.dispatch(SyncAction{});
                            }
                        }
                    };

                return { std::move(s), eff };
            }
            );
    }
}
