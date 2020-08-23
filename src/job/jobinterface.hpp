
#pragma once
#include "basejob.hpp"

namespace Kazv
{
    struct JobInterface
    {
        virtual ~JobInterface() = default;
        virtual std::future<BaseJob::Response> fetch(const BaseJob &job) = 0;
    };

}
