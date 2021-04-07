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

#include <sdk-model.hpp>

using namespace Kazv;
using IAr = boost::archive::text_iarchive;
using OAr = boost::archive::text_oarchive;

TEST_CASE("Serialize SdkModel", "[client][serialization]")
{
    SdkModel m1;
    SdkModel m2;

    std::stringstream stream;

    {
        auto ar = OAr(stream);
        ar << m1;
    }

    {
        auto ar = IAr(stream);
        ar >> m2;
    }

}
