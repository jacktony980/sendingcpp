
#include "types.hpp"
#include <variant>

namespace Kazv
{
    struct Error
    {
        struct NoError {};
        using JsonError = JsonWrap;
        std::variant<NoError, JsonError, std::string> error;

        struct SetErrorAction {
            std::variant<NoError, JsonError, std::string> error;
        };

        using Action = std::variant<SetErrorAction>;

        static inline Error update(Error, Action a) {
            return Error{std::get<SetErrorAction>(a).error};
        }

        constexpr bool hasError() const {
            return !std::holds_alternative<NoError>(error);
        }

        constexpr bool isJson() const {
            return !std::holds_alternative<JsonError>(error);
        }
    };
};
