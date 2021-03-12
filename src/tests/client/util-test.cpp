/*
 * Copyright (C) 2021 Tusooa Zhu <tusooa@kazv.moe>
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

#include <libkazv-config.hpp>


#include <catch2/catch.hpp>

#include <clientutil.hpp>

#include <immer/flex_vector.hpp>

namespace
{
    struct Model
    {
        int value;
        std::string data;
    };

    bool operator==(Model a, Model b)
    {
        return a.value == b.value && a.data == b.data;
    }

    bool operator!=(Model a, Model b)
    {
        return !(a == b);
    }
}

using namespace Kazv;

TEST_CASE("sortedUniqueMerge should behave correctly", "[client][clientutil]")
{
    immer::flex_vector<Model> base = {{50, "foo"}, {100, "bar"}};
    immer::flex_vector<Model> addon = {{1, "a"}, {2, "b"}, {70, "c"}, {110, "d"}};

    auto exists = [base](auto m) { return std::find(base.begin(), base.end(), m) != base.end(); };
    auto key = [](auto m) { return m.value; };

    auto res = sortedUniqueMerge(base, addon, exists, key);

    REQUIRE( res == immer::flex_vector<Model>{{1, "a"}, {2, "b"}, {50, "foo"}, {70, "c"}, {100, "bar"}, {110, "d"}} );
}
