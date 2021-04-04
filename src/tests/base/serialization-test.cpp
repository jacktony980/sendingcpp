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

#include <sstream>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>

#include <serialization/immer-flex-vector.hpp>
#include <serialization/immer-map.hpp>
#include <serialization/immer-box.hpp>
#include <serialization/immer-array.hpp>

//using namespace Kazv;
using IAr = boost::archive::text_iarchive;
using OAr = boost::archive::text_oarchive;

TEST_CASE("Serialize immer::array", "[base][serialization]")
{
    immer::array<int> v{1, 2, 3, 4, 5};
    immer::array<int> v2{50};

    std::stringstream stream;

    {
        auto ar = OAr(stream);
        ar << v;
    }

    {
        auto ar = IAr(stream);
        ar >> v2;
    }

    REQUIRE(v == v2);
}


TEST_CASE("Serialize immer::flex_vector", "[base][serialization]")
{
    immer::flex_vector<int> v{1, 2, 3, 4, 5};
    immer::flex_vector<int> v2{50};

    std::stringstream stream;

    {
        auto ar = OAr(stream);
        ar << v;
    }

    {
        auto ar = IAr(stream);
        ar >> v2;
    }

    REQUIRE(v == v2);
}

TEST_CASE("Serialize immer::map", "[base][serialization]")
{
    auto v = immer::map<int, int>{}
         .set(1, 6)
         .set(2, 7)
         .set(3, 8)
         .set(4, 9)
         .set(5, 10);
    auto v2 = immer::map<int, int>{}.set(10, 7);

    std::stringstream stream;

    {
        auto ar = OAr(stream);
        ar << v;
    }

    {
        auto ar = IAr(stream);
        ar >> v2;
    }

    REQUIRE(v == v2);
}

TEST_CASE("Serialize immer::box", "[base][serialization]")
{

    auto v = immer::box<int>{42};
    immer::box<int> v2;

    std::stringstream stream;

    {
        auto ar = OAr(stream);
        ar << v;
    }

    {
        auto ar = IAr(stream);
        ar >> v2;
    }

    REQUIRE(v == v2);
}
