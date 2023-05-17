#include "did.hpp"
#include <curl/curl.h>
#include "didModel.hpp"
#include "nlohmann/json.hpp"
#include <openssl/evp.h>
#include <openssl/ecdsa.h>
#include <openssl/obj_mac.h>

namespace Kazv
{
    using json = nlohmann::json;

    HttpRequest::HttpRequest() {
        curl_global_init(CURL_GLOBAL_ALL);
    }

    HttpRequest::~HttpRequest() {
        curl_global_cleanup();
    }

    DidListResponse HttpRequest::getDidList(const std::string& homeserver, const std::string& address) {
        CURL* curl = curl_easy_init();
        std::string response;

        if (curl) {
            std::string url = homeserver + "/_api/client/unstable/address/" + address;
            curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeCallback);
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

            CURLcode res = curl_easy_perform(curl);
            if (res != CURLE_OK) {
                fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
            }

            curl_easy_cleanup(curl);
        }

        json jsonData = json::parse(response);

        DidListResponse model;
        model.data = jsonData["data"].get<std::vector<std::string>>();

        return model;
    }

    PreloginResponse HttpRequest::prelogin(const std::string& homeserver, const std::string& data) {
        CURL* curl = curl_easy_init();
        std::string response;

        if (curl) {
            std::string url = homeserver + "/_api/client/unstable/did/pre_login1";
            curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
            curl_easy_setopt(curl, CURLOPT_POST, 1L);
            curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data.c_str());
            curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, data.size());
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeCallback);
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

            CURLcode res = curl_easy_perform(curl);
            if (res != CURLE_OK) {
                fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
            }

            curl_easy_cleanup(curl);
        }

        json jsonData = json::parse(response);

        PreloginResponse model;
        model.did = jsonData["did"].get<std::string>();
        model.message = jsonData["message"].get<std::string>();
        model.updated = jsonData["updated"].get<std::string>();
        return model;
    }

    LoginResponse HttpRequest::didlogin(const std::string& homeserver, const std::string& data) {
        CURL* curl = curl_easy_init();
        std::string response;

        if (curl) {
            std::string url = homeserver + "/_api/client/unstable/did/login";
            curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
            curl_easy_setopt(curl, CURLOPT_POST, 1L);
            curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data.c_str());
            curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, data.size());
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeCallback);
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

            CURLcode res = curl_easy_perform(curl);
            if (res != CURLE_OK) {
                fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
            }

            curl_easy_cleanup(curl);
        }

        json jsonData = json::parse(response);

        LoginResponse model;
        model.access_token = jsonData["access_token"].get<std::string>();
        model.user_id = jsonData["user_id"].get<std::string>();
        model.device_id = jsonData["device_id"].get<std::string>();

        return model;

    }

    std::string HttpRequest::web3Sign(const std::string& privateKey, const std::string& message) {
        EC_KEY* ecKey = EC_KEY_new_by_curve_name(NID_secp256k1);
        const BIGNUM* privKeyBN = BN_new();

        BN_hex2bn(&privKeyBN, privateKey.c_str());

        EC_KEY_set_private_key(ecKey, privKeyBN);

        EVP_MD_CTX* ctx = EVP_MD_CTX_create();
        EVP_PKEY* pkey = EVP_PKEY_new();
        EVP_PKEY_assign_EC_KEY(pkey, ecKey);

        EVP_MD_CTX_init(ctx);
        EVP_DigestSignInit(ctx, NULL, EVP_sha256(), NULL, pkey);

        EVP_DigestSignUpdate(ctx, message.c_str(), message.length());
        size_t sigLen;
        EVP_DigestSignFinal(ctx, NULL, &sigLen);

        std::vector<unsigned char> signature(sigLen);
        EVP_DigestSignFinal(ctx, signature.data(), &sigLen);

        std::string hexSignature;
        for (const auto& byte : signature) {
            char hex[3];
            sprintf(hex, "%02x", byte);
            hexSignature += hex;
        }

        EVP_MD_CTX_free(ctx);
        EVP_PKEY_free(pkey);
        EC_KEY_free(ecKey);
        BN_free(const_cast<BIGNUM*>(privKeyBN));

        return hexSignature;
    }

    size_t HttpRequest::writeCallback(void* contents, size_t size, size_t nmemb, std::string* response) {
        size_t totalSize = size * nmemb;
        response->append(static_cast<char*>(contents), totalSize);
        return totalSize;
    }
}
