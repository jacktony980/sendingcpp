/*
 * Copyright (C) 2020-2021 Tusooa Zhu <tusooa@vista.aero>
 *
 * This file is part of libkazv.
 *
 * libkazv is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as
 * published by the Free Software Foundation, either version 3 of the
 * License, or (at your option) any later version.
 *
 * libkazv is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with libkazv.  If not, see <https://www.gnu.org/licenses/>.
 */

#include <libkazv-config.hpp>


#include <string>
#include <iostream>
#include <fstream>
#include <lager/store.hpp>
#include <lager/event_loop/boost_asio.hpp>
#include <lager/resources_path.hpp>
#include <boost/asio.hpp>

#ifndef NDEBUG
//#include <lager/debug/http_server.hpp>
#endif

#include <asio-promise-handler.hpp>

#include <cprjobhandler.hpp>
#include <lagerstoreeventemitter.hpp>
#include <sdk.hpp>

#include "commands.hpp"

using namespace std::string_literals;

int main(int argc, char *argv[])
{

    if (argc <= 1) {
        std::cerr << "Usage: basicexample <auth-file-name>\n\n"
                  << "auth file is a text file with these lines:\n"
                  << "mode(pw or token)\n"
                  << "homeserver address\n"
                  << "username (if pw) or userid (if token)\n"
                  << "password (if pw) or token (if token)\n"
                  << "deviceId (if token) or blank (if pw)\n\n"
                  << "For example:\n"
                  << "pw\n"
                  << "https://some.server.org\n"
                  << "someUserName\n"
                  << "somePa$$w0rd\n";
        return 1;
    }

    boost::asio::io_context ioContext;
    auto eventEmitter =
        Kazv::LagerStoreEventEmitter(lager::with_boost_asio_event_loop{ioContext.get_executor()});

    Kazv::CprJobHandler jobHandler{ioContext.get_executor()};

    auto sdk = Kazv::makeDefaultEncryptedSdk(
        static_cast<Kazv::CprJobHandler &>(jobHandler),
        static_cast<Kazv::EventInterface &>(eventEmitter),
        Kazv::AsioPromiseHandler{ioContext.get_executor()},
        zug::identity
        );

    auto store = sdk.context();
    auto c = sdk.client();

    auto watchable = eventEmitter.watchable();
    watchable.after<Kazv::ReceivingRoomTimelineEvent>(
        [](auto e) {
            auto [event, roomId] = e;
            std::cout << "\033[1;32mreceiving event " << event.id()
                      << " in " << roomId
                      << " from " << event.sender()
                      << ": " << event.content().get().dump() << "\033[0m"
                      << std::endl;
        });
    watchable.after<Kazv::ReceivingToDeviceMessage>(
        [](auto e) {
            auto [event] = e;
            std::cout << "\033[1;32mreceiving to-device event "
                      << event.originalJson().get().dump()
                      << "\033[0m"
                      << std::endl;
        });

    {
        std::ifstream auth(argv[1]);
        if (! auth) {
            std::cerr << "Cannot open auth file " << argv[1] << "\n";
            return 1;
        }
        std::string mode;
        std::string homeserver;
        std::string username;
        std::getline(auth, mode);
        std::getline(auth, homeserver);
        std::getline(auth, username);

        if (mode == "token") {
            std::string token;
            std::string deviceId;
            std::getline(auth, token);
            std::getline(auth, deviceId);
            c.tokenLogin(homeserver, username, token, deviceId);
        } else {
            std::string password;
            std::getline(auth, password);
            c.passwordLogin(homeserver, username, password, "libkazv basic example");
            std::cout << "password login action sent" << std::endl;
        }

    }

    std::cout << "starting event loop" << std::endl;
    std::thread([&] { ioContext.run(); }).detach();

    std::size_t command = 1;
    while (true) {
        std::cout << "\033[1;33mCommand[" << command << "]: \033[0m\n";
        std::string l;
        if (! std::getline(std::cin, l)) {
            break;
        }
        parse(l, c);
        ++command;
    }

    jobHandler.stop();
}
