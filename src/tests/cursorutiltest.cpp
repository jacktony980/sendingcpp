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

#include <string>
#include <tuple>
#include <catch2/catch.hpp>

#include <immer/array.hpp>
#include <immer/array_transient.hpp>
#include <immer/map.hpp>

#include <client/cursorutil.hpp>

using namespace Kazv;

TEST_CASE("cursorutil should perform correctly", "[cursorutil]")
{
    immer::array<int> src1{4, 8, 7, 6};
    immer::array<std::string> exp1{"4", "8", "7", "6"};
    auto res1 = intoImmer(
        immer::array<std::string>{},
        zug::map(
            [](int x) {
                return std::to_string(x);
            }),
        src1);

    REQUIRE( exp1 == res1 );

    immer::map<int, int> src2;
    src2 = std::move(src2).set(4, 8).set(7, 6);
    immer::map<std::string, int> exp2;
    exp2 = std::move(exp2).set("4", 8).set("7", 6);
    auto res2 = intoImmer(
        immer::map<std::string, int>{},
        zug::map(
            [](auto x) {
                return std::make_pair(std::to_string(x.first), x.second);
            }),
        src2);

    REQUIRE( exp2 == res2 );

    immer::map<std::string, int> res3;

    auto rf =
        [&](auto st, auto in) {
            *st = in;
            return st;
        };

    containerMap(
        immer::map<std::string, int>{},
        zug::map(
            [](auto x) {
                return std::make_pair(std::to_string(x.first), x.second);
            }))(rf)(&res3, src2);

    REQUIRE( res3 == exp2 );
}
