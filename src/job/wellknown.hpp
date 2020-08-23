
#pragma once

#include "basejob.hpp"
#include "types.hpp"

namespace Kazv
{
    class GetWellKnownJob : public BaseJob
    {
    public:
        GetWellKnownJob(std::string serverUrl);
    };
}
