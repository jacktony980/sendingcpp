
#pragma once

#include <variant>
#include <tuple>
#include <functional>
#include <string>
#include <map>
#include <future>
#include <immer/map.hpp>
#include <immer/array.hpp>
#include <immer/box.hpp>

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

        class Query : public std::vector<std::pair<std::string, std::string>>
        {
            using BaseT = std::vector<std::pair<std::string, std::string>>;
        public:
            using BaseT::BaseT;
            void add(std::string k, std::string v) {
                push_back({k, v});
            }
        };

        using BytesBody = Bytes;
        using JsonBody = JsonWrap;
        struct EmptyBody {};
        using Body = std::variant<EmptyBody, JsonBody, BytesBody>;

        static constexpr bool isBodyJson(const Body &body) {
            return std::holds_alternative<JsonBody>(body);
        };

        using Header = immer::box<std::map<std::string, std::string>>;

        using StatusCode = int;
        struct Response {
            StatusCode statusCode;
            Body body;
            Header header;
        };

        static constexpr bool success(const Response &res) {
            return res.statusCode < 400;
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
                Query query = {},
                Header header = {});

        bool shouldReturnJson() const;

        std::string url() const;

        BytesBody requestBody() const;

        Header requestHeader() const;

        ReturnType returnType() const;

        /// returns the non-encoded query as an array of pairs
        Query requestQuery() const;

        Method requestMethod() const;

        static bool contentTypeMatches(immer::array<std::string> expected, std::string actual);

    private:
        struct Private;
        Descendent<Private> m_d;
    };

    JsonWrap jsonBody(const BaseJob::Response &res);



    namespace detail
    {
        template<class T>
        struct AddToQueryT
        {
            template<class U>
            static void call(BaseJob::Query &q, std::string name, U &&arg) {
                q.add(name, std::to_string(std::forward<U>(arg)));
            }
        };

        template<>
        struct AddToQueryT<std::string>
        {
            template<class U>
            static void call(BaseJob::Query &q, std::string name, U &&arg) {
                q.add(name, std::forward<U>(arg));
            }
        };

        template<>
        struct AddToQueryT<bool>
        {
            template<class U>
            static void call(BaseJob::Query &q, std::string name, U &&arg) {
                q.add(name, std::forward<U>(arg) ? "true"s : "false"s);
            }
        };

        template<class T>
        struct AddToQueryT<immer::array<T>>
        {
            template<class U>
            static void call(BaseJob::Query &q, std::string name, U &&arg) {
                for (auto v : std::forward<U>(arg)) {
                    q.add(name, v);
                }
            }
        };

        template<>
        struct AddToQueryT<json>
        {
            // https://github.com/nlohmann/json/issues/2040
            static void call(BaseJob::Query &q, std::string /* name */, const json &arg) {
                // assume v is string type
                for (auto [k, v] : arg.items()) {
                    q.add(k, v);
                }
            }
        };
    }

    template<class T>
    inline void addToQuery(BaseJob::Query &q, std::string name, T &&arg)
    {
        detail::AddToQueryT<std::decay_t<T>>::call(q, name, std::forward<T>(arg));
    }

    namespace detail
    {
        template<class T>
        struct AddToQueryIfNeededT
        {
            template<class U>
            static void call(BaseJob::Query &q, std::string name, U &&arg) {
                if constexpr (detail::hasEmptyMethod(arg)) {
                    if (! arg.empty()) {
                        addToQuery(q, name, std::forward<U>(arg));
                    }
                } else {
                    addToQuery(q, name, std::forward<U>(arg));
                }
            }
        };

        template<class T>
        struct AddToQueryIfNeededT<std::optional<T>>
        {
            template<class U>
            static void call(BaseJob::Query &q, std::string name, U &&arg) {
                if (arg.has_value()) {
                    addToQuery(q, name, std::forward<U>(arg).value());
                }
            }
        };
    }

    template<class T>
    inline void addToQueryIfNeeded(BaseJob::Query &q, std::string name, T &&arg)
    {
        detail::AddToQueryIfNeededT<std::decay_t<T>>::call(q, name, std::forward<T>(arg));
    }

}
