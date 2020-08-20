
#include "wellknown.hpp"

namespace Kazv
{
    GetWellKnownJob::GetWellKnownJob(std::string serverUrl)
        : BaseJob(std::move(serverUrl), "/.well-known/matrix/client", GET)
    {
    }
}
