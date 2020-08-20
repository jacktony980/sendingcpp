
#include "types.hpp"

namespace Kazv
{
    struct Result {
        Status status;
        std::optional<std::string> homeserver;
        std::optional<std::string> identityServer;
    };

    Result parseHomeserver(Response r);

    auto GetWellKnownJob::parse(Response r) -> Result
    {
        if (r.statusCode == 404) {
            return Result{SUCC};
        } else if (r.statusCode != 200
                   || !isBodyJson(r.body)) {
            return Result{FAIL};
        } else {
            return Result{FAIL};
        }
    }

}
