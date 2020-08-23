
#pragma once

#include "jobinterface.hpp"

namespace Kazv
{
    struct CprJobHandler : public JobInterface
    {
        ~CprJobHandler() override;
        std::future<BaseJob::Response> fetch(const BaseJob &job) override;
    };
}
