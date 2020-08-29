
#include <string>
#include <iostream>
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
            std::cout << "receiving event " << event.id()
                      << " in " << roomId
                      << " from " << event.sender()
                      << ": " << event.content().get().dump() << std::endl;
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

    ioContext.run();
}
