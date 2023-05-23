#pragma once


#include <string>
#include "sdnModel.hpp"

namespace Kazv
{
    class SDNHttpRequest {
    public:
        SDNHttpRequest();
        ~SDNHttpRequest();

        std::string homeserver;
        std::string access_token;

        std::string inviteUserToRoom(const std::string& roomId, const std::string& data);
        std::string sendMessage(const std::string& roomId, const std::string& data);

    private:
        static size_t writeCallback(void* contents, size_t size, size_t nmemb, std::string* response);
    };

    
}