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

#include <crypto.hpp>
#include <outbound-group-session.hpp>
#include <random-generator.hpp>

using namespace Kazv;

TEST_CASE("Deterministic constructors of Crypto and OutboundGroupSession", "[crypto][deterministic]")
{
    auto rg = RandomInterface{RandomDeviceGenerator{}};
    auto random = rg.generateRange<RandomData>(Crypto::constructRandomSize());
    Crypto crypto1(RandomTag{}, random);
    Crypto crypto2(RandomTag{}, random);

    REQUIRE(crypto1.toJson() == crypto2.toJson());

    random = rg.generateRange<RandomData>(OutboundGroupSession::constructRandomSize());
    auto creationTime = currentTimeMs();

    OutboundGroupSession ogs1(RandomTag{}, random, creationTime);
    OutboundGroupSession ogs2(RandomTag{}, random, creationTime);

    REQUIRE(nlohmann::json(ogs1) == nlohmann::json(ogs2));
}
