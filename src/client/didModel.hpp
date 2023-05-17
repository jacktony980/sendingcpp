#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <nlohmann/json.hpp>

namespace Kazv
{
    using json = nlohmann::json;

    class DidListResponse {
    public:
        std::vector<std::string> data;
    };

    class PreloginRequest {
    public:
        std::string did;
        std::string address;

        PreloginRequest(const std::string& d, const std::string& a) : did(d), address(a) {}

        std::string toString() const {
            json j;
            j["did"] = did;
            j["address"] = address;
            return j.dump();
        }
    };

    class PreloginResponse {
    public:
        std::string did;
        std::string message;
        std::string updated;
    };


    class LoginRequest {
    public:
        std::string type;
        std::string updated;
        std::string identifier;

        LoginRequest(const std::string& u, const std::string& i) : updated(u), identifier(i) {}

        std::string toString() const {
            json j;
            j["type"] = "m.login.did.identity";
            j["updated"] = updated;
            j["identifier"] = identifier;
            return j.dump();
        }
    };

    class LoginResponse {
    public:
        std::string access_token;
        std::string user_id;
        std::string device_id;
    };

    class IdentifierModel {
    public:
        std::string did;
        std::string address;
        std::string token;

        IdentifierModel(const std::string& d, const std::string& a, const std::string& t) : did(d), address(a), token(t) {}

        std::string toString() const {
            json j;
            j["did"] = did;
            j["address"] = address;
            j["token"] = token;
            return j.dump();
        }
    };
}

