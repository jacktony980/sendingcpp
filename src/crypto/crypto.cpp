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

#include <vector>

#include <zug/transducer/filter.hpp>

#include <olm/olm.h>

#include <nlohmann/json.hpp>

#include <debug.hpp>
#include <event.hpp>
#include <cursorutil.hpp>

#include "crypto-p.hpp"
#include "session-p.hpp"

#include "crypto-util.hpp"
#include "time-util.hpp"

namespace Kazv
{
    using namespace CryptoConstants;

    CryptoPrivate::CryptoPrivate()
        : accountData(olm_account_size(), 0)
        , account(olm_account(accountData.data()))
        , utilityData(olm_utility_size(), '\0')
        , utility(olm_utility(utilityData.data()))
    {
        auto randLen = olm_create_account_random_length(account);
        auto randomData = genRandom(randLen);
        checkError(olm_create_account(account, randomData.data(), randLen));
    }

    CryptoPrivate::~CryptoPrivate()
    {
        olm_clear_account(account);
    }

    CryptoPrivate::CryptoPrivate(const CryptoPrivate &that)
        : accountData(olm_account_size(), 0)
        , account(olm_account(accountData.data()))
        , uploadedOneTimeKeysCount(that.uploadedOneTimeKeysCount)
        , numUnpublishedKeys(that.numUnpublishedKeys)
        , knownSessions(that.knownSessions)
        , outboundSessions(that.outboundSessions)
        , inboundGroupSessions(that.inboundGroupSessions)
        , outboundGroupSessions(that.outboundGroupSessions)
        , utilityData(olm_utility_size(), '\0')
        , utility(olm_utility(utilityData.data()))
    {
        unpickle(that.pickle());
    }

    ByteArray CryptoPrivate::pickle() const
    {
        auto key = ByteArray(3, 'x');
        auto pickleData = ByteArray(olm_pickle_account_length(account), '\0');
        checkError(olm_pickle_account(account, key.data(), key.size(),
                                      pickleData.data(), pickleData.size()));
        return pickleData;
    }

    void CryptoPrivate::unpickle(ByteArray pickleData)
    {
        auto key = ByteArray(3, 'x');
        checkError(olm_unpickle_account(account, key.data(), key.size(),
                                        pickleData.data(), pickleData.size()));
    }

    std::size_t CryptoPrivate::checkError(std::size_t code) const
    {
        if (code == olm_error()) {
            kzo.crypto.warn() << "Olm error: " << olm_account_last_error(account) << std::endl;
        }
        return code;
    }

    std::size_t CryptoPrivate::checkUtilError(std::size_t code) const
    {
        if (code == olm_error()) {
            kzo.crypto.warn() << "Olm utility error: " << olm_utility_last_error(utility) << std::endl;
        }
        return code;
    }


    MaybeString CryptoPrivate::decryptOlm(nlohmann::json content)
    {
        auto theirCurve25519IdentityKey = content.at("sender_key").get<std::string>();

        auto ourCurve25519IdentityKey = curve25519IdentityKey();

        if (! content.at("ciphertext").contains(ourCurve25519IdentityKey)) {
            return NotBut("Message not intended for us");
        }

        auto type = content.at("ciphertext").at(ourCurve25519IdentityKey).at("type").get<int>();
        auto body = content.at("ciphertext").at(ourCurve25519IdentityKey).at("body").get<std::string>();

        auto hasKnownSession = knownSessions.find(theirCurve25519IdentityKey) != knownSessions.end();

        if (type == 0) { // pre-key message
            bool shouldCreateNewSession =
                // there is no possible session
                (! hasKnownSession)
                // the possible session does not match this message
                || (! knownSessions.at(theirCurve25519IdentityKey).matches(body));

            if (shouldCreateNewSession) {
                auto created = createInboundSession(theirCurve25519IdentityKey, body);
                if (! created) { // cannot create session, thus cannot decrypt
                    return NotBut("Cannot create session");
                }
            }

            auto &session = knownSessions.at(theirCurve25519IdentityKey);

            return session.decrypt(type, body);
        } else {
            if (! hasKnownSession) {
                return NotBut("No available session");
            }

            auto &session = knownSessions.at(theirCurve25519IdentityKey);
            return session.decrypt(type, body);
        }
    }

    MaybeString CryptoPrivate::decryptMegOlm(nlohmann::json eventJson)
    {
        auto content = eventJson.at("content");

        auto senderKey = content.at("sender_key").get<std::string>();
        auto sessionId = content.at("session_id").get<std::string>();
        auto roomId = eventJson.at("room_id").get<std::string>();

        auto k = KeyOfGroupSession{roomId, senderKey, sessionId};

        if (inboundGroupSessions.find(k) == inboundGroupSessions.end()) {
            return NotBut("We do not have the keys for this");
        } else {
            auto msg = content.at("ciphertext").get<std::string>();
            auto eventId = eventJson.at("event_id").get<std::string>();
            auto originServerTs = eventJson.at("origin_server_ts").get<Timestamp>();
            auto &session = inboundGroupSessions.at(k);

            return session.decrypt(msg, eventId, originServerTs);
        }
    }

    bool CryptoPrivate::createInboundSession(std::string theirCurve25519IdentityKey,
                                             std::string message)
    {
        auto s = Session(InboundSessionTag{}, account,
                         theirCurve25519IdentityKey, message);

        if (s.valid()) {
            checkError(olm_remove_one_time_keys(account, s.m_d->session));
            knownSessions.insert_or_assign(theirCurve25519IdentityKey, std::move(s));
            return true;
        }

        return false;
    }

    bool CryptoPrivate::reuseOrCreateOutboundGroupSession(std::string roomId, MegOlmSessionRotateDesc desc)
    {
        auto it = outboundGroupSessions.find(roomId);
        bool valid = true;
        if (it == outboundGroupSessions.end()) {
            valid = false;
        } else {
            auto &session = it->second;
            if (currentTimeMs() - session.creationTimeMs() >= desc.ms) {
                valid = false;
            } else if (session.messageIndex() >= desc.messages) {
                valid = false;
            }
        }
        if (! valid) {
            outboundGroupSessions.insert_or_assign(roomId, OutboundGroupSession());
            auto &session = outboundGroupSessions.at(roomId);
            auto sessionId = session.sessionId();
            auto sessionKey = session.sessionKey();
            auto senderKey = curve25519IdentityKey();

            auto k = KeyOfGroupSession{roomId, senderKey, sessionId};

            if (! createInboundGroupSession(k, sessionKey, ed25519IdentityKey())) {
                kzo.client.warn() << "Create inbound group session from outbound group session failed. We may not be able to read our own messages." << std::endl;
            }
        }
        return valid;
    }


    Crypto::Crypto()
        : m_d(new CryptoPrivate{})
    {
    }

    Crypto::~Crypto() = default;

    Crypto::Crypto(const Crypto &that)
        : m_d(new CryptoPrivate(*that.m_d))
    {
    }

    Crypto::Crypto(Crypto &&that)
        : m_d(std::move(that.m_d))
    {
    }

    Crypto &Crypto::operator=(const Crypto &that)
    {
        m_d.reset(new CryptoPrivate(*that.m_d));
        return *this;
    }

    Crypto &Crypto::operator=(Crypto &&that)
    {
        m_d = std::move(that.m_d);
        return *this;
    }

    ByteArray CryptoPrivate::identityKeys()
    {
        auto ret = ByteArray(olm_account_identity_keys_length(account), '\0');
        checkError(olm_account_identity_keys(account, ret.data(), ret.size()));
        return ret;
    }

    std::string CryptoPrivate::ed25519IdentityKey()
    {
        auto keys = identityKeys();
        auto keyStr = std::string(keys.begin(), keys.end());
        auto keyJson = nlohmann::json::parse(keyStr);
        return keyJson.at(ed25519);
    }

    std::string CryptoPrivate::curve25519IdentityKey()
    {
        auto keys = identityKeys();
        auto keyStr = std::string(keys.begin(), keys.end());
        auto keyJson = nlohmann::json::parse(keyStr);
        return keyJson.at(curve25519);
    }

    std::string Crypto::ed25519IdentityKey()
    {
        return m_d->ed25519IdentityKey();
    }

    std::string Crypto::curve25519IdentityKey()
    {
        return m_d->curve25519IdentityKey();
    }

    std::string Crypto::sign(nlohmann::json j)
    {
        j.erase("signatures");
        j.erase("unsigned");

        auto str = j.dump();

        auto ret = ByteArray(olm_account_signature_length(m_d->account), '\0');

        kzo.crypto.dbg() << "We are about to sign: " << str << std::endl;

        m_d->checkError(olm_account_sign(m_d->account,
                                         str.data(), str.size(),
                                         ret.data(), ret.size()));

        return std::string{ret.begin(), ret.end()};
    }

    void Crypto::setUploadedOneTimeKeysCount(immer::map<std::string /* algorithm */, int> uploadedOneTimeKeysCount)
    {
        m_d->uploadedOneTimeKeysCount = uploadedOneTimeKeysCount;
    }

    int Crypto::maxNumberOfOneTimeKeys()
    {
        return olm_account_max_number_of_one_time_keys(m_d->account);
    }

    void Crypto::genOneTimeKeys(int num)
    {
        auto random = genRandom(olm_account_generate_one_time_keys_random_length(m_d->account, num));
        auto res = m_d->checkError(
            olm_account_generate_one_time_keys(
                m_d->account,
                num,
                random.data(), random.size()));

        if (res != olm_error()) {
            m_d->numUnpublishedKeys += num;
        }
    }

    nlohmann::json Crypto::unpublishedOneTimeKeys()
    {
        auto keys = ByteArray(olm_account_one_time_keys_length(m_d->account), '\0');
        m_d->checkError(olm_account_one_time_keys(m_d->account, keys.data(), keys.size()));

        return nlohmann::json::parse(std::string(keys.begin(), keys.end()));
    }

    void Crypto::markOneTimeKeysAsPublished()
    {
        auto ret = m_d->checkError(olm_account_mark_keys_as_published(m_d->account));
        if (ret != olm_error()) {
            m_d->numUnpublishedKeys = 0;
        }
    }

    int Crypto::numUnpublishedOneTimeKeys() const
    {
        return m_d->numUnpublishedKeys;
    }

    int Crypto::uploadedOneTimeKeysCount(std::string algorithm) const
    {
        return m_d->uploadedOneTimeKeysCount[algorithm];
    }

    MaybeString Crypto::decrypt(nlohmann::json eventJson)
    {
        auto content = eventJson.at("content");
        auto algo = content.at("algorithm").get<std::string>();
        if (algo == olmAlgo) {
            return m_d->decryptOlm(std::move(content));
        } else if (algo == megOlmAlgo) {
            return m_d->decryptMegOlm(eventJson);
        }
        return NotBut("Algorithm " + algo + " not supported");
    }

    bool Crypto::createInboundGroupSession(KeyOfGroupSession k, std::string sessionKey, std::string ed25519Key)
    {
        return m_d->createInboundGroupSession(std::move(k), std::move(sessionKey), std::move(ed25519Key));
    }

    bool CryptoPrivate::createInboundGroupSession(KeyOfGroupSession k, std::string sessionKey, std::string ed25519Key)
    {
        auto session = InboundGroupSession(sessionKey, ed25519Key);
        if (session.valid()) {
            inboundGroupSessions.insert_or_assign(k, std::move(session));
            return true;
        }
        return false;
    }

    bool Crypto::verify(nlohmann::json object, std::string userId, std::string deviceId, std::string ed25519Key)
    {
        if (! object.contains("signatures")) {
            return false;
        }
        std::string signature;
        try {
            signature = object.at("signatures").at(userId).at(ed25519 + ":" + deviceId);
        } catch(const std::exception &) {
            return false;
        }
        object.erase("signatures");
        object.erase("unsigned");

        auto message = object.dump();

        auto res = m_d->checkUtilError(
            olm_ed25519_verify(m_d->utility,
                               ed25519Key.c_str(), ed25519Key.size(),
                               message.c_str(), message.size(),
                               signature.data(), signature.size()));

        return res != olm_error();
    }

    MaybeString Crypto::getInboundGroupSessionEd25519KeyFromEvent(const nlohmann::json &eventJson) const
    {
        auto content = eventJson.at("content");

        auto senderKey = content.at("sender_key").get<std::string>();
        auto sessionId = content.at("session_id").get<std::string>();
        auto roomId = eventJson.at("room_id").get<std::string>();

        auto k = KeyOfGroupSession{roomId, senderKey, sessionId};

        if (m_d->inboundGroupSessions.find(k) == m_d->inboundGroupSessions.end()) {
            return NotBut("We do not have the keys for this");
        } else {
            auto &session = m_d->inboundGroupSessions.at(k);
            return session.ed25519Key();
        }
    }

    nlohmann::json Crypto::encryptOlm(nlohmann::json eventJson, std::string userId, std::string deviceId)
    {
        try {
            auto &session = m_d->outboundSessions.at(KeyOfOutboundSession{userId, deviceId});
            auto [type, body] = session.encrypt(eventJson.dump());
            return nlohmann::json{
                {
                    session.theirIdentityKey(), {
                        {"type", type},
                        {"body", body}
                    }
                }
            };
        } catch (const std::exception &) {
            return nlohmann::json::object();
        }
    }

    std::pair<nlohmann::json, std::optional<std::string>>
    Crypto::encryptMegOlm(nlohmann::json eventJson, MegOlmSessionRotateDesc desc)
    {
        auto roomId = eventJson.at("room_id").get<std::string>();
        auto content = eventJson.at("content");
        auto type = eventJson.at("type").get<std::string>();

        auto jsonToEncrypt = nlohmann::json::object();
        jsonToEncrypt["room_id"] = roomId;
        jsonToEncrypt["content"] = std::move(content);
        jsonToEncrypt["type"] = type;

        auto textToEncrypt = std::move(jsonToEncrypt).dump();

        auto reused = m_d->reuseOrCreateOutboundGroupSession(roomId, desc);
        auto &session = m_d->outboundGroupSessions.at(roomId);
        // sessionKey() returns the key for the next message to be decrypted
        // so we need to call it before encrypt()
        auto sessionKey = session.sessionKey();

        auto ciphertext = session.encrypt(std::move(textToEncrypt));

        return {
            json{
                {"algorithm", CryptoConstants::megOlmAlgo},
                {"sender_key", curve25519IdentityKey()},
                {"ciphertext", ciphertext},
                {"session_id", session.sessionId()},
            },
            reused ? std::nullopt : std::optional<std::string>(sessionKey)
        };
    }

    void Crypto::forceRotateMegOlmSession(std::string roomId)
    {
        // just let the session expire 0ms after creation and
        // we will have a new one
        m_d->reuseOrCreateOutboundGroupSession(std::move(roomId), MegOlmSessionRotateDesc());
    }

    auto Crypto::devicesMissingOutboundSessionKey(UserIdToDeviceIdMap userIdToDeviceIdMap) const -> UserIdToDeviceIdMap
    {
        auto ret = UserIdToDeviceIdMap{};
        for (auto [userId, devices] : userIdToDeviceIdMap) {
            auto unknownDevices =
                intoImmer(immer::flex_vector<std::string>{},
                          zug::filter([=](auto deviceId) {
                                          auto k = KeyOfOutboundSession{userId, deviceId};
                                          return m_d->outboundSessions.find(k) == m_d->outboundSessions.end();
                                      }),
                          devices);
            if (! unknownDevices.empty()) {
                ret = std::move(ret).set(userId, std::move(unknownDevices));
            }
        }
        return ret;
    }

    void Crypto::createOutboundSession(std::string userId, std::string deviceId, std::string theirIdentityKey,
                                       std::string theirOneTimeKey)
    {
        // TODO
        auto session = Session(OutboundSessionTag{},
                               m_d->account,
                               theirIdentityKey,
                               theirOneTimeKey);

        if (session.valid()) {
            m_d->outboundSessions.insert_or_assign(KeyOfOutboundSession{userId, deviceId},
                                                   std::move(session));
        }
    }
}
