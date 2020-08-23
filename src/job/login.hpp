
#include "basejob.hpp"

namespace Kazv
{
    class GetLoginFlowsJob : public BaseJob
    {
    public:
        GetLoginFlowsJob(std::string serverUrl);
    };
    class LoginJob : public BaseJob
    {
    public:
        LoginJob(std::string serverUrl,
                 std::string username,
                 std::string password,
                 std::optional<std::string> deviceName);
    };
};
