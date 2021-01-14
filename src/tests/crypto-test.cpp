/*
 * Copyright (C) 2020 Tusooa Zhu <tusooa@vista.aero>
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


#include <catch2/catch.hpp>

#include <crypto/crypto.hpp>

using namespace Kazv;

TEST_CASE("Crypto should be copyable", "[crypto]")
{
    Crypto crypto;
    Crypto cryptoClone(crypto);

    REQUIRE(crypto.identityKeys() == cryptoClone.identityKeys());
    REQUIRE(crypto.ed25519IdentityKey() == cryptoClone.ed25519IdentityKey());
    REQUIRE(crypto.curve25519IdentityKey() == cryptoClone.curve25519IdentityKey());
}
