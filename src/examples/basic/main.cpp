
#include <string>
#include <iostream>
#include <lager/store.hpp>
#include <lager/event_loop/boost_asio.hpp>
#include <boost/asio.hpp>

#include <client/client.hpp>
#include <job/cprjobhandler.hpp>
#include <eventemitter/lagerstoreeventemitter.hpp>

using namespace std::string_literals;

int main()
{
    boost::asio::io_context ioContext;
    auto eventEmitter =
        Kazv::LagerStoreEventEmitter(lager::with_boost_asio_event_loop{ioContext.get_executor()});

    Kazv::Descendent<Kazv::JobInterface> jobHandler(Kazv::CprJobHandler{ioContext.get_executor()});
    auto store = lager::make_store<Kazv::Client::Action>(
        Kazv::Client{},
        &Kazv::Client::update,
        lager::with_boost_asio_event_loop{ioContext.get_executor()},
        lager::with_deps(
            std::ref(*jobHandler.data()),
            std::ref(static_cast<Kazv::EventInterface &>(eventEmitter))));

    std::string homeserver;
    std::string username;
    std::string password;
    std::cout << "Homeserver: ";
    std::getline(std::cin, homeserver);
    std::cout << "Username: ";
    std::getline(std::cin, username);
    std::cout << "Password: ";
    std::getline(std::cin, password);
    store.dispatch(Kazv::Client::LoginAction{homeserver, username, password, "libkazv basic example"s});

    auto watchable = eventEmitter.watchable();
    watchable.after<Kazv::ReceivingRoomTimelineEvent>(
        [](auto &&e) {
            auto [event, roomId] = e;
            std::cout << "receiving event in " << roomId << ": " << event.get().dump() << std::endl;
        });


    ioContext.run();
}
