
#include <string>
#include <iostream>
#include <fstream>
#include <lager/store.hpp>
#include <lager/event_loop/boost_asio.hpp>
#include <boost/asio.hpp>

#ifndef NDEBUG
#include <lager/debug/debugger.hpp>
#include <lager/debug/http_server.hpp>
#endif

#include <client/client.hpp>
#include <job/cprjobhandler.hpp>
#include <eventemitter/lagerstoreeventemitter.hpp>
#include "commands.hpp"

using namespace std::string_literals;

int main(int argc, char *argv[])
{
    boost::asio::io_context ioContext;
    auto eventEmitter =
        Kazv::LagerStoreEventEmitter(lager::with_boost_asio_event_loop{ioContext.get_executor()});

    Kazv::Descendent<Kazv::JobInterface> jobHandler(Kazv::CprJobHandler{ioContext.get_executor()});

#ifndef NDEBUG
    auto debugger = lager::http_debug_server{argc, (const char **)argv, 8080, "./_deps/lager-src/resources"};
#endif
    auto store = lager::make_store<Kazv::Client::Action>(
        Kazv::Client{},
        &Kazv::Client::update,
        lager::with_boost_asio_event_loop{ioContext.get_executor()},
        lager::with_deps(
            std::ref(*jobHandler.data()),
            std::ref(static_cast<Kazv::EventInterface &>(eventEmitter)))
#ifndef NDEBUG
        , lager::with_debugger(debugger)
#endif
        );

    if (argc <= 1) {
        std::cerr << "Usage: basicexample <auth-file-name>\n\n"
                  << "auth file is a text file with these three lines:\n"
                  << "homeserver address\n"
                  << "username\n"
                  << "password\n\n"
                  << "For example:\n"
                  << "https://some.server.org\n"
                  << "someUserName\n"
                  << "somePa$$w0rd\n";
        return 1;
    }

    std::string homeserver;
    std::string username;
    std::string password;
    {
        std::ifstream auth(argv[1]);
        if (! auth) {
            std::cerr << "Cannot open auth file " << argv[1] << "\n";
            return 1;
        }
        std::getline(auth, homeserver);
        std::getline(auth, username);
        std::getline(auth, password);
    }
    store.dispatch(Kazv::Client::LoginAction{homeserver, username, password, "libkazv basic example"s});

    auto watchable = eventEmitter.watchable();
    watchable.after<Kazv::ReceivingRoomTimelineEvent>(
        [](auto &&e) {
            auto [event, roomId] = e;
            std::cout << "\033[1;32mreceiving event " << event.id()
                      << " in " << roomId
                      << " from " << event.sender()
                      << ": " << event.content().get().dump() << "\033[0m"
                      << std::endl;
        });

    /*lager::reader<Kazv::RoomList> roomList = store
        .zoom(lager::lenses::attr(&Kazv::Client::roomList));

    lager::watch(
        roomList,
        [](Kazv::RoomList l) {
            std::cout << "Room list updated." << std::endl;
            std::cout << "Rooms: ";
            for (auto [id, room]: l.rooms) {
                std::cout << id << " ";
            }
            std::cout << std::endl;
            });*/

    std::thread([&] { ioContext.run(); }).detach();

    lager::reader<Kazv::Client> c = store
#ifndef NDEBUG
        // get the client from the debugger
        .xform(zug::map([](auto m) -> Kazv::Client { return m; }))
#endif
        ;

    std::size_t command = 1;
    while (true) {
        std::cout << "\033[1;33mCommand[" << command << "]: \033[0m\n";
        std::string l;
        std::getline(std::cin, l);
        std::optional<Kazv::Client::Action> a = intent(l, c);
        ++command;
    }
}
