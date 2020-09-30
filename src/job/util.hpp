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

#include "basejob.hpp"

namespace Kazv
{
    /// Returns whether we should retry for this (unsuccessful) event,
    /// and the suggested interval in ms before retrying from the server.
    /// If the server has not provided a suggested interval, -1 will be
    /// used to indicate that the client should choose an interval instead.
    inline std::pair<bool, int> shouldRetryFor(BaseJob::Response res) {
        bool shouldRetry = false;
        int retryAfterMs = -1;
        bool errorCodeFromJson = false;
        if (BaseJob::isBodyJson(res.body)) {
            const auto &j = jsonBody(res).get();
            if (j.contains("errcode"s)) {
                errorCodeFromJson = true;
                auto code = j["errcode"s];
                if (code == "M_LIMIT_EXCEEDED"s) {
                    shouldRetry = true;
                    if (j.contains("retry_after_ms"s)) {
                        retryAfterMs = j["retry_after_ms"s];
                    }
                }
            }
        }
        if (! errorCodeFromJson) {
            auto c = res.statusCode;
            if (c == 408 // Request Timeout
                || c == 429 // Too many requests
                || c == 500 // Internal Server Error
                || c == 502 // Bad Gateway
                || c == 503 // Service Unavailable
                || c == 504 // Gateway Timeout
                ) {
                shouldRetry = true;
            }
        }
        return {shouldRetry, retryAfterMs};
    }
}
