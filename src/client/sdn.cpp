#include "sdn.hpp"
#include <curl/curl.h>
#include "sdnModel.hpp"
#include "nlohmann/json.hpp"

namespace Kazv
{
    using json = nlohmann::json;

    SDNHttpRequest::SDNHttpRequest() {
        curl_global_init(CURL_GLOBAL_ALL);
    }

    SDNHttpRequest::~SDNHttpRequest() {
        curl_global_cleanup();
    }

    std::string SDNHttpRequest::inviteUserToRoom(const std::string& roomId, const std::string& data) {
        CURL* curl = curl_easy_init();
        std::string response;

        if (curl) {
            std::string url = homeserver + "/_api/client/r0/rooms/" + roomId + "/invite";
            std::cout << "inviteUserToRoom url " << url << "\n" << "data: " << data << std::endl;
            curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
            curl_easy_setopt(curl, CURLOPT_POST, 1L);
            curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data.c_str());
            curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, data.size());
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeCallback);
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
            // 设置请求头部信息
            struct curl_slist* headers = nullptr;
            // headers = curl_slist_append(headers, "Content-Type: application/json");
            std::string authorization = "Authorization: Bearer " + access_token;
            headers = curl_slist_append(headers, authorization.c_str());
            curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

            CURLcode res = curl_easy_perform(curl);
            if (res != CURLE_OK) {
                fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
            }
            curl_easy_cleanup(curl);
        }
        std::cout << "inviteUserToRoom response " << response << "\n" << std::endl;
        return response;
    }

    std::string SDNHttpRequest::sendMessage(const std::string& roomId, const std::string& data) {
        CURL* curl = curl_easy_init();
        std::string response;
        if (curl) {
            std::string url = homeserver + "/_api/client/r0/rooms/" + roomId + "/send/m.room.message/123";
            std::cout << "sendMessage url " << url << "\n" << "data: " << data << std::endl;

            curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
            curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "PUT");
            curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data.c_str());
            curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, data.size());
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeCallback);
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
            // 设置请求头部信息
            struct curl_slist* headers = nullptr;
            // headers = curl_slist_append(headers, "Content-Type: application/json");
            std::string authorization = "Authorization: Bearer " + access_token;
            headers = curl_slist_append(headers, authorization.c_str());
            curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

            CURLcode res = curl_easy_perform(curl);
            if (res != CURLE_OK) {
                std::cerr << "Failed to perform curl request: " << curl_easy_strerror(res) << std::endl;
            }
            curl_easy_cleanup(curl);
        }
        std::cout << "sendMessage response " << response << "\n" << std::endl;
        return response;
    }

    size_t SDNHttpRequest::writeCallback(void* contents, size_t size, size_t nmemb, std::string* response) {
        size_t totalSize = size * nmemb;
        response->append(static_cast<char*>(contents), totalSize);
        return totalSize;
    }
}