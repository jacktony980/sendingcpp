/*
 * This file is part of libkazv.
 * SPDX-FileCopyrightText: 2020-2021 Tusooa Zhu <tusooa@kazv.moe>
 * SPDX-License-Identifier: AGPL-3.0-or-later
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
#include <client/client-model.hpp>
#include <client/did.hpp>
#include <client/didModel.hpp>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

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

    auto sdk = Kazv::makeDefaultSdkWithCryptoRandom(
        Kazv::genRandomData(Kazv::makeDefaultSdkWithCryptoRandomSize()),
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

        Kazv::HttpRequest request;
        // std::string homeserver = "https://sdktest.sending.me/";
        //get did list
        std::string address = "0xdb4F2CF56621D2E07c752dd4ea2A5c89c146A682";
        Kazv::DidListResponse getResponse = request.getDidList(homeserver,address);

        std::string did = "";
        if (getResponse.data.size() > 0) {
            //login with did
            //pre login
            Kazv::PreloginRequest preloginRequest = Kazv::PreloginRequest(getResponse.data[0],"");
            Kazv::PreloginResponse postResponse = request.prelogin(homeserver, preloginRequest.toString());

            //login
            Kazv::IdentifierModel identifierModel = Kazv::IdentifierModel(getResponse.data[0],"",request.web3Sign("257c82fd4267527d56bb0facdafb138bf021621704616e4d81b12d492b44f054",postResponse.message));
            Kazv::LoginRequest loginRequest = Kazv::LoginRequest(postResponse.updated,identifierModel.toString());
            Kazv::LoginResponse loginResponse = request.didlogin(homeserver, loginRequest.toString());
            printf("login success: userId = %s, token = %s\n", loginResponse.user_id.c_str(), loginResponse.access_token.c_str());
            // c.serverUrl = homeserver;
            // c.userId = loginResponse.user_id;
            // c.token = loginResponse.access_token;
            // c.deviceId = loginResponse.device_id;
            // c.loggedIn = true;
        } else {
            //login with address
            //pre login
            Kazv::PreloginRequest preloginRequest = Kazv::PreloginRequest("","did:pkh:eip155:1:" + address);
            Kazv::PreloginResponse postResponse = request.prelogin(homeserver, preloginRequest.toString());

            //login
            Kazv::IdentifierModel identifierModel = Kazv::IdentifierModel("","did:pkh:eip155:1:" + address,request.web3Sign("257c82fd4267527d56bb0facdafb138bf021621704616e4d81b12d492b44f054",postResponse.message));
            Kazv::LoginRequest loginRequest = Kazv::LoginRequest(postResponse.updated,identifierModel.toString());
            Kazv::LoginResponse loginResponse = request.didlogin(homeserver, loginRequest.toString());
            printf("login success: userId = %s, token = %s\n", loginResponse.user_id.c_str(), loginResponse.access_token.c_str());
            // c.serverUrl() = homeserver;
            // c.userId() = loginResponse.user_id;
            // c.token() = loginResponse.access_token;
            // c.deviceId() = loginResponse.device_id;
            // c.loggedIn() = true;
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



