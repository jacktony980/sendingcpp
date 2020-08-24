
#include <lager/util.hpp>
#include <vector>
#include <tuple>

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
                Query query,
                Header header);
        std::string fullRequestUrl;
        Method method;
        ReturnType returnType;
        BytesBody body;
        Query query;
        Header header;
    };

    BaseJob::Private::Private(std::string serverUrl,
                              std::string requestUrl,
                              Method method,
                              std::string token,
                              ReturnType returnType,
                              Body body,
                              Query query,
                              Header header)
        : fullRequestUrl(serverUrl + requestUrl)
        , method(std::move(method))
        , returnType(returnType)
        , body()
        , query(std::move(query))
    {
        auto header_ = header.get();
        if (token.size()) {
            header_["Authorization"] = "Bearer " + token;
        }

        if (isBodyJson(body)) {
            JsonBody j = std::get<JsonBody>(std::move(body));
            header_["Content-Type"] = "application/json";
            this->body = j.get().dump();
        } else if (std::holds_alternative<BytesBody>(body)) {
            BytesBody b = std::get<BytesBody>(std::move(body));
            this->body = b;
        }

        this->header = header_;
    }

    BaseJob::BaseJob(std::string serverUrl,
                     std::string requestUrl,
                     Method method,
                     std::string token,
                     ReturnType returnType,
                     Body body,
                     Query query,
                     Header header)
        : m_d(std::move(Private(serverUrl, requestUrl, method, token, returnType, body, query, header)))
    {
    }

    bool BaseJob::shouldReturnJson() const
    {
        return m_d->returnType == ReturnType::Json;
    };

    std::string BaseJob::url() const
    {
        return m_d->fullRequestUrl;
    };

    auto BaseJob::requestBody() const -> BytesBody
    {
        return m_d->body;
    }

    auto BaseJob::requestHeader() const -> Header
    {
        return m_d->header;
    }

    auto BaseJob::returnType() const -> ReturnType
    {
        return m_d->returnType;
    }

    auto BaseJob::requestQuery() const -> Query
    {
        return m_d->query;
    }

    auto BaseJob::requestMethod() const -> Method
    {
        return m_d->method;
    }

    JsonWrap jsonBody(const BaseJob::Response &res)
    {
        return std::get<JsonWrap>(res.body);
    }

    bool BaseJob::contentTypeMatches(immer::array<std::string> expected, std::string actual)
    {
        for (const auto &i : expected) {
            if (i == "*/*"s) {
                return true;
            } else {
                std::size_t pos = i.find("/*"s);
                if (pos != std::string::npos) {
                    std::string majorType(i.data(), i.data() + pos + 1); // includes `/'
                    if (actual.find(majorType) == 0) {
                        return true;
                    }
                } else if (i == actual) {
                    return true;
                }
            }
        }
        return false;
    }
}
