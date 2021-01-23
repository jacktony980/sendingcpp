/*
 * Copyright (C) 2020 Tusooa Zhu
 *
 * This file is part of libkazv.
 *
 * libkazv is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as
 * published by the Free Software Foundation, either version 3 of the
 * License, or (at your option) any later version.
 *
 * libkazv is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with libkazv.  If not, see <https://www.gnu.org/licenses/>.
 */


#pragma once
#include <libkazv-config.hpp>

#ifndef NDEBUG
#include <lager/debug/cereal/struct.hpp>
#endif

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

    constexpr inline bool operator==(Error::NoError, Error::NoError)
    {
        return true;
    }

    inline bool operator==(Error a, Error b)
    {
        return a.error == b.error;
    }
#ifndef NDEBUG
    LAGER_CEREAL_STRUCT(Error::SetErrorAction);
#endif
    LAGER_CEREAL_STRUCT(Error::NoError);
    template<class Archive>
    void serialize(Archive &ar, Error &m, std::uint32_t const /*version*/)
    {
        ar(m.error);
    }
}

CEREAL_CLASS_VERSION(Kazv::Error, 0);
