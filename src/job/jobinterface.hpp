
#pragma once
#include <functional>

#include "basejob.hpp"

namespace Kazv
{
    struct JobInterface
    {
        virtual ~JobInterface() = default;
        virtual void async(std::function<void()> func) = 0;
        /// makes an async fetch.
        /// callback will not block the current thread.
        virtual void fetch(const BaseJob &job, std::function<void(std::shared_future<BaseJob::Response>)> callback) = 0;
    };

}
