
#include <libkazv-config.hpp>
#include <sdk.hpp>
#include <cprjobhandler.hpp>
#include <lagerstoreeventemitter.hpp>
#include <asio-promise-handler.hpp>

#include <iostream>
#include <cstdlib>
#include <lager/store.hpp>
#include <lager/event_loop/boost_asio.hpp>
#include <boost/asio.hpp>

int main()
{
    auto io = boost::asio::io_context{};
    auto jh = Kazv::CprJobHandler{io.get_executor()};
    auto ee = Kazv::LagerStoreEventEmitter(lager::with_boost_asio_event_loop{io.get_executor()});


    auto sdk = Kazv::makeSdk(
        Kazv::SdkModel{},
        jh,
        ee,
        Kazv::AsioPromiseHandler{io.get_executor()},
        zug::identity
        );

    auto client = sdk.client();

    auto prompt =
        [](std::string question) {
            std::cout << question << std::endl;
            std::string ret;
            std::getline(std::cin, ret);
            return ret;
        };

    auto homeserver = prompt("Homeserver:");
    auto username = prompt("Username:");
    auto password = prompt("Password:");
    auto deviceName = std::string("libkazv example");

    client.passwordLogin(homeserver, username, password, deviceName)
        .then([=](auto status) {
            if (! status.success()) {
                std::cerr << "Login failed" << std::endl;
                std::exit(1);
            }
            client.startSyncing();
        });

    io.run();
}
