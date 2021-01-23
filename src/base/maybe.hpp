/*
 * Copyright (C) 2021 Tusooa Zhu <tusooa@vista.aero>
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
#include "libkazv-config.hpp"

#include <string>
#include <optional>

namespace Kazv
{
    class NotBut
    {
        std::string m_reason;
    public:
        explicit NotBut(std::string r) : m_reason(r) {}
        std::string reason() const { return m_reason; }
    };

    template<class T>
    class Maybe : public std::optional<T>
    {
        std::string m_reason;
        using BaseT = std::optional<T>;
    public:
        Maybe(T x) : BaseT(x) {}
        Maybe(NotBut r) : BaseT(), m_reason(r.reason()) {}

        std::string reason() const { return m_reason; }
    };

    using MaybeString = Maybe<std::string>;
}
