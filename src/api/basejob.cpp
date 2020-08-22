
#include <cpr/cpr.h>
#include <cpr/util.h>
#include <lager/util.hpp>

#include "basejob.hpp"

namespace Kazv
{
    BaseJob::Get BaseJob::GET{};
    BaseJob::Post BaseJob::POST{};
    BaseJob::Put BaseJob::PUT{};
    BaseJob::Delete BaseJob::DELETE{};

    struct BaseJob::Private
    {
        Private(std::string serverUrl,
                std::string requestUrl,
                Method method,
                std::string token,
                ReturnType returnType,
                Body body,
                Query query);
        std::string fullRequestUrl;
        Method method;
        ReturnType returnType;
        cpr::Header header;
        BytesBody body;
        cpr::Parameters params;
    };

    BaseJob::Private::Private(std::string serverUrl,
                              std::string requestUrl,
                              Method method,
                              std::string token,
                              ReturnType returnType,
                              Body body,
                              Query query)
        : fullRequestUrl(serverUrl + requestUrl)
        , method(std::move(method))
        , returnType(returnType)
        , header()
        , body()
        , params()
    {
        if (token.size()) {
            header["Authorization"] = "Bearer " + token;
        }

        if (isBodyJson(body)) {
            JsonBody j = std::get<JsonBody>(std::move(body));
            header["Content-Type"] = "application/json";
            this->body = j.get().dump();
        } else if (std::holds_alternative<BytesBody>(body)) {
            BytesBody b = std::get<BytesBody>(std::move(body));
            this->body = b;
        }

        if (! query.empty()) {
            // from cpr/parameters.cpp
            cpr::CurlHolder holder;
            for (const auto kv : query) {
                std::string key = kv.first;
                std::string value = kv.second;
                params.AddParameter(cpr::Parameter(std::move(key), std::move(value)), holder);
            }
        }
    }

    BaseJob::BaseJob(std::string serverUrl,
                     std::string requestUrl,
                     Method method,
                     std::string token,
                     ReturnType returnType,
                     Body body,
                     Query query)
        : m_d(std::move(Private(serverUrl, requestUrl, method, token, returnType, body, query)))
    {
    }

    auto BaseJob::fetch() const -> std::future<Response>
    {
        cpr::Url url{m_d->fullRequestUrl};
        cpr::Body body(m_d->body);
        cpr::Header header{m_d->header};
        cpr::Parameters params(m_d->params);

        auto callback = [returnType = m_d->returnType](cpr::Response r) -> Response {
                            Body body = r.text;

                            if (returnType == ReturnType::Json) {
                                try {
                                    body = JsonBody(std::move(json::parse(r.text)));
                                } catch (const json::exception &) {
                                    // the response is not valid json
                                }
                            }

                            return { r.status_code, body };
                        };

        return std::visit(lager::visitor{
                [=](Get) {
                    return cpr::GetCallback(callback, url, header, body, params);
                },
                [=](Post) {
                    return cpr::PostCallback(callback, url, header, body, params);
                },
                [=](Put) {
                    return cpr::PutCallback(callback, url, header, body, params);
                },
                [=](Delete) {
                    return cpr::DeleteCallback(callback, url, header, body, params);
                }
                }, m_d->method);
    }

    bool BaseJob::shouldReturnJson() const
    {
        return m_d->returnType == ReturnType::Json;
    };

    std::string BaseJob::url() const
    {
        return m_d->fullRequestUrl;
    };

    JsonWrap jsonBody(const BaseJob::Response &res)
    {
        return std::get<JsonWrap>(res.body);
    }
}
