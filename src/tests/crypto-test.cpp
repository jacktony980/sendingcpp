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

#include <libkazv-config.hpp>


#include <catch2/catch.hpp>

#include <sstream>

#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>


#include <crypto/crypto.hpp>
#include <aes-256-ctr.hpp>


using namespace Kazv;
using namespace Kazv::CryptoConstants;

using IAr = boost::archive::text_iarchive;
using OAr = boost::archive::text_oarchive;

template<class T>
static void serializeDup(const T &in, T &out)
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
}

TEST_CASE("Crypto should be copyable", "[crypto]")
{
    Crypto crypto;
    crypto.genOneTimeKeys(1);
    auto oneTimeKeys = crypto.unpublishedOneTimeKeys();
    Crypto cryptoClone(crypto);

    REQUIRE(crypto.ed25519IdentityKey() == cryptoClone.ed25519IdentityKey());
    REQUIRE(crypto.curve25519IdentityKey() == cryptoClone.curve25519IdentityKey());

    auto oneTimeKeys2 = cryptoClone.unpublishedOneTimeKeys();

    REQUIRE(oneTimeKeys == oneTimeKeys2);
    REQUIRE(crypto.numUnpublishedOneTimeKeys() == cryptoClone.numUnpublishedOneTimeKeys());
}

TEST_CASE("Crypto should be serializable", "[crypto]")
{
    Crypto crypto;
    crypto.genOneTimeKeys(1);
    auto oneTimeKeys = crypto.unpublishedOneTimeKeys();

    Crypto cryptoClone;

    serializeDup(crypto, cryptoClone);

    REQUIRE(crypto.ed25519IdentityKey() == cryptoClone.ed25519IdentityKey());
    REQUIRE(crypto.curve25519IdentityKey() == cryptoClone.curve25519IdentityKey());

    auto oneTimeKeys2 = cryptoClone.unpublishedOneTimeKeys();

    REQUIRE(oneTimeKeys == oneTimeKeys2);
    REQUIRE(crypto.numUnpublishedOneTimeKeys() == cryptoClone.numUnpublishedOneTimeKeys());
}

TEST_CASE("Serialize Crypto with an OutboundGroupSession", "[crypto]")
{
    Crypto crypto;

    std::string roomId = "!example:example.org";
    auto desc = MegOlmSessionRotateDesc{500000 /* ms */, 100 /* messages */};

    crypto.rotateMegOlmSession(roomId);

    Crypto cryptoClone;

    serializeDup(crypto, cryptoClone);

    REQUIRE(! cryptoClone.rotateMegOlmSessionIfNeeded(roomId, desc).has_value());
}

TEST_CASE("Generating and publishing keys should work", "[crypto]")
{
    Crypto crypto;
    crypto.genOneTimeKeys(1);
    REQUIRE(crypto.numUnpublishedOneTimeKeys() == 1);

    crypto.genOneTimeKeys(1);
    REQUIRE(crypto.numUnpublishedOneTimeKeys() == 2);

    crypto.markOneTimeKeysAsPublished();
    REQUIRE(crypto.numUnpublishedOneTimeKeys() == 0);
}

TEST_CASE("Should reuse existing inbound session to encrypt", "[crypto]")
{
    Crypto a;
    Crypto b;

    a.genOneTimeKeys(1);

    // Get A publish the key and send to B
    auto k = a.unpublishedOneTimeKeys();
    a.markOneTimeKeysAsPublished();

    auto oneTimeKey = std::string{};

    for (auto [id, key] : k[curve25519].items()) {
        oneTimeKey = key;
    }

    auto aIdKey = a.curve25519IdentityKey();
    b.createOutboundSession(aIdKey, oneTimeKey);

    auto origJson = json{{"test", "mew"}};
    auto encryptedMsg = b.encryptOlm(origJson, aIdKey);
    auto encJson = json{
        {"content",
         {
             {"algorithm", olmAlgo},
             {"ciphertext", encryptedMsg},
             {"sender_key", b.curve25519IdentityKey()}
         }
        }
    };

    auto decryptedOpt = a.decrypt(encJson);

    REQUIRE(decryptedOpt);

    auto decryptedJson = json::parse(decryptedOpt.value());

    REQUIRE(decryptedJson == origJson);

    using StrMap = immer::map<std::string, std::string>;
    auto devMap = immer::map<std::string, StrMap>()
        .set("b", StrMap().set("dev", b.curve25519IdentityKey()));

    Crypto aClone{a};

    auto devices = a.devicesMissingOutboundSessionKey(devMap);
    auto devicesAClone = aClone.devicesMissingOutboundSessionKey(devMap);

    // No device should be missing an olm session, as A has received an
    // inbound olm session before.
    auto expected = immer::map<std::string, immer::flex_vector<std::string>>();

    REQUIRE(devices == expected);
    REQUIRE(devicesAClone == expected);
}

TEST_CASE("Encrypt and decrypt AES-256-CTR", "[crypto][aes256ctr]")
{
    auto key = genRandom(AES256CTRDesc::keySize);
    auto iv = genRandom(AES256CTRDesc::ivSize);

    auto desc = AES256CTRDesc(key, iv);
    auto desc2 = desc;

    std::string original = "test for aes-256-ctr";

    auto encrypted = desc.processInPlace(original);

    auto decrypted = desc2.processInPlace(encrypted);

    REQUIRE(original == decrypted);
}

TEST_CASE("Encrypt and decrypt AES-256-CTR with any sequence type", "[crypto][aes256ctr]")
{
    auto key = genRandom(AES256CTRDesc::keySize);
    auto iv = genRandom(AES256CTRDesc::ivSize);

    auto desc = AES256CTRDesc(key, iv);
    auto desc2 = desc;

    std::string oStr = "test for aes-256-ctr";
    std::vector<unsigned char> original(oStr.begin(), oStr.end());

    auto encrypted = desc.processInPlace(original);

    auto decrypted = desc2.processInPlace(encrypted);

    REQUIRE(original == decrypted);
}

TEST_CASE("Encrypt and decrypt AES-256-CTR in a non-destructive way", "[crypto][aes256ctr]")
{
    auto key = genRandom(AES256CTRDesc::keySize);
    auto iv = genRandom(AES256CTRDesc::ivSize);

    auto desc = AES256CTRDesc(key, iv);

    std::string original = "test for aes-256-ctr";

    auto [next, encrypted] = desc.process(original);

    auto [next2, encrypted2] = desc.process(original);

    REQUIRE(encrypted == encrypted2);

    auto [next3, decrypted] = desc.process(encrypted);

    REQUIRE(original == decrypted);
}

TEST_CASE("Encrypt and decrypt AES-256-CTR in batches", "[crypto][aes256ctr]")
{
    auto key = genRandom(AES256CTRDesc::keySize);
    auto iv = genRandom(AES256CTRDesc::ivSize);

    auto desc = AES256CTRDesc(key, iv);

    std::string original = "test for aes-256-ctr";

    std::string orig2 = "another test string...";

    auto [next, encrypted] = desc.process(original);
    auto [next2, encrypted2] = next.process(orig2);

    auto [next3, decrypted] = desc.process(encrypted + encrypted2);

    REQUIRE(original + orig2 == decrypted);
}
