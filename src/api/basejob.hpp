
#pragma once

#include <variant>
#include <tuple>
#include <functional>
#include <string>
#include <immer/map.hpp>
#include <immer/array.hpp>
#include <future>

#include "types.hpp"
#include "descendent.hpp"

namespace Kazv
{
    class BaseJob
    {
    public:
        struct Get {};
        struct Post {};
        struct Put {};
        struct Delete {};
        using Method = std::variant<Get, Post, Put, Delete>;

        static Get GET;
        static Post POST;
        static Put PUT;
        static Delete DELETE;

        using Query = immer::map<std::string, std::string>;

        using BytesBody = Bytes;
        using JsonBody = JsonWrap;
        struct EmptyBody {};
        using Body = std::variant<EmptyBody, JsonBody, BytesBody>;

        static constexpr bool isBodyJson(const Body &body) {
            return std::holds_alternative<JsonBody>(body);
        };

        using StatusCode = int;
        struct Response {
            StatusCode statusCode;
            Body body;
        };

        constexpr bool success(const Response &res) const {
            return res.statusCode < 300 && (!shouldReturnJson() || isBodyJson(res.body));
        }

        enum ReturnType {
            Json,
            Byte,
        };

        BaseJob(std::string serverUrl,
                std::string requestUrl,
                Method method,
                std::string token = {},
                ReturnType returnType = ReturnType::Json,
                Body body = EmptyBody{},
                Query query = {});

        std::future<Response> fetch() const;

        bool shouldReturnJson() const;

        std::string url() const;

    private:
        struct Private;
        Descendent<Private> m_d;
    };

    JsonWrap jsonBody(const BaseJob::Response &res);

    template<class T,
             std::enable_if_t<std::is_same_v<decltype(std::declval<T>().empty()), bool>,
                                    int> = 0>
    inline BaseJob::Query addToQueryIfNeeded(BaseJob::Query &&q, std::string name, T &&arg)
    {
        if (! arg.empty()) {
            return std::move(q).set(name, std::forward<T>(arg));
        }
        return std::move(q);
    };

    template<class T>
    inline BaseJob::Query addToQueryIfNeeded(BaseJob::Query &&q, std::string name, T &&arg)
    {
        return std::move(q).set(name, std::forward<T>(arg));
    };

    template<class T>
    inline BaseJob::Query addToQueryIfNeeded(BaseJob::Query &&q, std::string name, std::optional<T> &&arg)
    {
        if (arg.has_value()) {
            return std::move(q).set(name, std::forward<T>(arg).value());
        }
        return std::move(q);
    };
}
