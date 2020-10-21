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
#include <functional>

#include "basejob.hpp"

namespace Kazv
{
    struct JobInterface
    {
        virtual ~JobInterface() = default;
        virtual void async(std::function<void()> func) = 0;
        virtual void setTimeout(std::function<void()> func, int ms) = 0;
        virtual void setInterval(std::function<void()> func, int ms) = 0;
        /// makes an async fetch.
        /// callback will not block the current thread.
        virtual void fetch(const BaseJob &job, std::function<void(Response)> callback) = 0;
    };

}
