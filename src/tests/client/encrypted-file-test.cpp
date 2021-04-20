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

#include <encrypted-file.hpp>

using namespace Kazv;

using IAr = boost::archive::text_iarchive;
using OAr = boost::archive::text_oarchive;

template<class T>
static void serializeTest(const T &in, T &out)
{
    std::stringstream stream;

    {
        auto ar = OAr(stream);
        ar << in;
    }

    {
        auto ar = IAr(stream);
        ar >> out;
    }

    REQUIRE(in == out);
}

auto encryptedFile = R"({
      "url": "mxc://example.org/FHyPlCeYUSFFxlgbQYZmoEoe",
      "mimetype": "image/jpeg",
      "v": "v2",
      "key": {
        "alg": "A256CTR",
        "ext": true,
        "k": "aWF6-32KGYaC3A_FEUCk1Bt0JA37zP0wrStgmdCaW-0",
        "key_ops": ["encrypt","decrypt"],
        "kty": "oct"
      },
      "iv": "w+sE15fzSc0AAAAAAAAAAA",
      "hashes": {
        "sha256": "fdSLu/YkRx3Wyh3KQabP3rd6+SFiKg5lsJZQHtkSAYA"
      }
    })"_json;

std::string mxcUri = "mxc://example.org/FHyPlCeYUSFFxlgbQYZmoEoe";
std::string key = "aWF6-32KGYaC3A_FEUCk1Bt0JA37zP0wrStgmdCaW-0";
std::string iv = "w+sE15fzSc0AAAAAAAAAAA";
std::string sha256Hash = "fdSLu/YkRx3Wyh3KQabP3rd6+SFiKg5lsJZQHtkSAYA";

TEST_CASE("Convert between EncryptedFileDesc and EncryptedFile json object", "[client][encrypted-file]")
{
    auto desc = EncryptedFileDesc::fromJson(encryptedFile);

    REQUIRE(desc.key() == key);
    REQUIRE(desc.iv() == iv);
    REQUIRE(desc.sha256Hash() == sha256Hash);
    REQUIRE(desc.mxcUri() == mxcUri);

    auto j = desc.toJson();
    auto dup = EncryptedFileDesc::fromJson(j);
    REQUIRE(desc == dup);
}

TEST_CASE("EncryptedFileDesc is serializable", "[client][encrypted-file]")
{
    auto desc = EncryptedFileDesc::fromJson(encryptedFile);
    auto desc2 = EncryptedFileDesc();

    serializeTest(desc, desc2);
}

TEST_CASE("Moved-from EncryptedFileDesc should not throw exceptions", "[client][encrypted-file]")
{
    auto desc = EncryptedFileDesc::fromJson(encryptedFile);
    auto desc2 = std::move(desc);

    REQUIRE(desc.mxcUri() == "");
    REQUIRE(desc.key() == "");
    REQUIRE(desc.iv() == "");
    REQUIRE(desc.sha256Hash() == "");
}
