
#include <string>
#include <iostream>
#include <lager/store.hpp>
#include <lager/event_loop/boost_asio.hpp>
#include <boost/asio.hpp>

#include <client/client.hpp>
#include <job/cprjobhandler.hpp>

using namespace std::string_literals;

int main()
{
    boost::asio::io_context ioContext;

    Kazv::Descendent<Kazv::JobInterface> jobHandler(Kazv::CprJobHandler{ioContext.get_executor()});
    auto store = lager::make_store<Kazv::Client::Action>(
        Kazv::Client{},
        &Kazv::Client::update,
        lager::with_boost_asio_event_loop{ioContext.get_executor()},
        lager::with_deps(std::ref(*jobHandler.data())));

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



    ioContext.run();
    std::cout << "Token: " << store.get().token << std::endl;
}
