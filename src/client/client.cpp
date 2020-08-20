
#include <lager/util.hpp>
#include <lager/context.hpp>
#include <functional>

#include "client.hpp"
#include "api/login.hpp"
#include "types.hpp"
#include "iostream"

namespace Kazv
{
    lager::effect<Client::Action> loginEffect(Client::LoginAction a)
    {
        return
            [=](auto &&ctx) {
                LoginJob job(a.serverUrl,
                             a.username,
                             a.password,
                             a.deviceName);
                auto res = job.fetch();
                std::cout << "Result validity: " << res.valid() << std::endl;
                auto r = res.get();
                if (job.success(r)) {
                    std::cout << "Job success" << std::endl;
                    const json &j = jsonBody(r).get();
                    try {
                        std::string serverUrl = j.contains("well_known")
                            ? j.at("well_known").at("m.homeserver").at("base_url").get<std::string>()
                            : a.serverUrl;
                        ctx.dispatch(Client::LoadUserInfoAction{
                                serverUrl,
                                j.at("user_id"),
                                j.at("access_token"),
                                j.at("device_id"),
                                /* loggedIn = */ true
                                });
                    } catch (const json::out_of_range &e) {
                        std::cout << "Bad login" << e.what() << std::endl;
                        ctx.dispatch(Error::SetErrorAction{e.what()});
                    }
                }
            };
    }

    lager::effect<Client::Action> logoutEffect(Client::LogoutAction a)
    {
        return
            [=](auto &&ctx) {
                ctx.dispatch(Client::LoadUserInfoAction{
                        ""s,
                        ""s,
                        ""s,
                        ""s,
                        /* loggedIn = */ true
                    });
            };
    }


    auto Client::update(Client m, Action a) -> Result
    {
        std::cout << "Client::update()" << std::endl;
        return
            std::visit(lager::visitor{
                [=](Error::Action a) mutable -> Result {
                    m.error = Error::update(m.error, a);
                    return {std::move(m), lager::noop};
                },
                [=](LoginAction a) mutable -> Result {
                    return {std::move(m), loginEffect(std::move(a))};
                },
                [=](LogoutAction a) mutable -> Result {
                    return {std::move(m), logoutEffect(std::move(a))};
                },
                [=](LoadUserInfoAction a) mutable -> Result {
                    std::cout << "LoadUserInfoAction: " << a.userId << std::endl;
                    // Need to use set()s
                    Client newClient{a.serverUrl,
                                     a.userId,
                                     a.token,
                                     a.deviceId,
                                     a.loggedIn,
                                     Error::NoError{}};
                    return { std::move(newClient), lager::noop };
                },
            }, std::move(a));
    }
}
