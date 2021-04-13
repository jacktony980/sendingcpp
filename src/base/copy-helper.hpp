/*
 * Copyright (C) 2020-2021 Tusooa Zhu <tusooa@kazv.moe>
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

#define KAZV_DECLARE_COPYABLE(typeName)         \
    typeName(const typeName &that);             \
    typeName(typeName &&that);                  \
    typeName &operator=(const typeName &that);  \
    typeName &operator=(typeName &&that);

#define KAZV_DEFINE_COPYABLE_UNIQUE_PTR(typeName, privateName)          \
    typeName::typeName(const typeName &that)                            \
        : privateName(std::make_unique<decltype(privateName)::element_type>(*(that.privateName))) \
    {                                                                   \
    }                                                                   \
    typeName::typeName(typeName &&that)                                 \
        : privateName(std::move(that.privateName))                      \
    {                                                                   \
    }                                                                   \
    typeName &typeName::operator=(const typeName &that)                 \
    {                                                                   \
        privateName.reset();                                            \
        privateName = std::make_unique<decltype(privateName)::element_type>(*(that.privateName)); \
        return *this;                                                   \
    }                                                                   \
    typeName &typeName::operator=(typeName &&that)                      \
    {                                                                   \
        if (m_d != that.m_d) {                                          \
            privateName.reset();                                        \
            privateName = std::move(that.privateName);                  \
        }                                                               \
        return *this;                                                   \
    }
