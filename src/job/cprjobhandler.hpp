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

#include <memory>

#include <boost/asio.hpp>

#include "jobinterface.hpp"
#include "descendent.hpp"

namespace Kazv
{
    struct CprJobHandler : public JobInterface
    {
        CprJobHandler(boost::asio::io_context::executor_type executor);
        ~CprJobHandler() override;
        void async(std::function<void()> func) override;
        void setTimeout(std::function<void()> func, int ms,
                        std::optional<std::string> timerId = std::nullopt) override;
        void setInterval(std::function<void()> func, int ms,
                         std::optional<std::string> timerId = std::nullopt) override;
        void cancel(std::string timerId) override;

        void submit(BaseJob job,
                    std::function<void(Response)> callback) override;

        void stop();
    private:
        struct Private;
        std::unique_ptr<Private> m_d;
    };
}
