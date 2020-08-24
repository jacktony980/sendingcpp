
#include <lager/util.hpp>
#include <lager/context.hpp>
#include <functional>

#include "client.hpp"
#include "csapi/login.hpp"
#include "types.hpp"
#include "debug.hpp"
#include "job/jobinterface.hpp"

namespace Kazv
{
    Client::Effect loginEffect(Client::LoginAction a)
    {
        return
            [=](auto &&ctx) {
                LoginJob job(a.serverUrl,
                             "m.login.password"s, // type
                             UserIdentifier{ "m.id.user"s, json{{"user", a.username}} }, // identifier
                             a.password,
                             {}, // token, not used
                             {}, // device id, not used
                             a.deviceName.value_or("libkazv"));
                auto &jobHandler = lager::get<JobInterface &>(ctx);
                jobHandler.fetch(
                    job,
                    [=](std::shared_future<BaseJob::Response> res) {
                        auto r = res.get();
                        if (LoginJob::success(r)) {
                            dbgClient << "Job success" << std::endl;
                            const json &j = jsonBody(r).get();
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
                            // after user info is loaded, do first sync
                            ctx.dispatch(Client::SyncAction{});
                        }
                    });
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
        dbgClient << "Client::update()" << std::endl;
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
                [=](SyncAction a) -> Result {
                    return {m, syncEffect(m, a)};
                },
                [=](LoadUserInfoAction a) mutable -> Result {
                    dbgClient << "LoadUserInfoAction: " << a.userId << std::endl;

                    m.serverUrl = a.serverUrl;
                    m.userId = a.userId;
                    m.token = a.token;
                    m.deviceId = a.deviceId;
                    m.loggedIn = a.loggedIn;

                    return { std::move(m),
                             [](auto &&ctx) {
                                 ctx.dispatch(Error::SetErrorAction{Error::NoError{}});
                             }
                    };
                },
            }, std::move(a));
    }
}
