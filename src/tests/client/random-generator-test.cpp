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

#include <string>
#include <vector>

#include <random-generator.hpp>

using namespace Kazv;

TEST_CASE("RandomInterface with a const generator", "[client][random]")
{
    auto generator = [] { return 1; };
    auto iface = RandomInterface{generator};

    REQUIRE(iface() == 1);

    auto range = iface.generateRange<std::vector<unsigned int>>(5);
    REQUIRE(range == std::vector<unsigned int>(5, 1));

    auto range2 = iface.fillRange(std::vector<unsigned int>(5));
    REQUIRE(range2 == std::vector<unsigned int>(5, 1));
}

TEST_CASE("RandomInterface with a non-const generator", "[client][random]")
{
    unsigned int cur = 0;
    auto generator = [&cur] { ++cur; return cur; };
    auto iface = RandomInterface{generator};

    REQUIRE(iface() == 1);

    auto range = iface.generateRange<std::vector<unsigned int>>(5);
    REQUIRE(range == std::vector<unsigned int>{2, 3, 4, 5, 6});

    auto range2 = iface.fillRange(std::vector<unsigned int>(5));
    REQUIRE(range2 == std::vector<unsigned int>{7, 8, 9, 10, 11});
}

TEST_CASE("RandomInterface generating ranges with element type narrower than unsigned int", "[client][random]")
{
    auto generator = [] { return (static_cast<unsigned char>(-1) + 1) + 1; };
    auto iface = RandomInterface{generator};

    auto range = iface.generateRange<std::string>(5);
    REQUIRE(range == std::string(5, 1));
}
