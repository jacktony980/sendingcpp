/*
 * Copyright (C) 2021 Tusooa Zhu <tusooa@vista.aero>
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

#include <olm/olm.h>

#include <debug.hpp>

#include "crypto-p.hpp"
#include "session-p.hpp"


namespace Kazv
{
    std::size_t SessionPrivate::checkError(std::size_t code) const
    {
        if (code == olm_error()) {
            kzo.crypto.warn() << "Olm session error: " << olm_session_last_error(session) << std::endl;
        }
        return code;
    }

    SessionPrivate::SessionPrivate()
        : sessionData(olm_session_size(), '\0')
        , session(olm_session(sessionData.data()))
    {
    }

    SessionPrivate::SessionPrivate(OutboundSessionTag,
                                   OlmAccount *acc,
                                   std::string theirIdentityKey,
                                   std::string theirOneTimeKey)
        : SessionPrivate()
    {
        this->theirIdentityKey = theirIdentityKey;

        auto random = genRandom(olm_create_outbound_session_random_length(session));

        auto res = checkError(olm_create_outbound_session(
                                  session,
                                  acc,
                                  theirIdentityKey.c_str(), theirIdentityKey.size(),
                                  theirOneTimeKey.c_str(), theirOneTimeKey.size(),
                                  random.data(), random.size()));

        if (res != olm_error()) {
            valid = true;
        }
    }

    SessionPrivate::SessionPrivate(InboundSessionTag,
                   OlmAccount *acc,
                   std::string theirIdentityKey,
                   std::string message)
        : SessionPrivate()
    {
        auto res = checkError(olm_create_inbound_session_from(
                                  session,
                                  acc,
                                  theirIdentityKey.c_str(), theirIdentityKey.size(),
                                  message.data(), message.size()));

        if (res != olm_error()) {
            valid = true;
        }
    }

    SessionPrivate::SessionPrivate(const SessionPrivate &that)
        : SessionPrivate()
    {
        theirIdentityKey = that.theirIdentityKey;
        valid = unpickle(that.pickle());
    }

    ByteArray SessionPrivate::pickle() const
    {
        auto pickleData = ByteArray(olm_pickle_session_length(session), '\0');
        auto key = ByteArray(3, 'x');
        checkError(olm_pickle_session(session,
                                      key.data(), key.size(),
                                      pickleData.data(), pickleData.size()));
        return pickleData;
    }

    bool SessionPrivate::unpickle(ByteArray pickleData)
    {
        auto key = ByteArray(3, 'x');
        auto res = checkError(olm_unpickle_session(
                                   session,
                                   key.data(), key.size(),
                                   pickleData.data(), pickleData.size()));

        return res != olm_error();
    }

    Session::Session()
        : m_d(new SessionPrivate)
    {
    }

    Session::Session(OutboundSessionTag,
                     OlmAccount *acc,
                     std::string theirIdentityKey,
                     std::string theirOneTimeKey)
        : m_d(new SessionPrivate{
                OutboundSessionTag{},
                acc,
                theirIdentityKey,
                theirOneTimeKey})
    {
    }

    Session::Session(InboundSessionTag,
                     OlmAccount *acc,
                     std::string theirIdentityKey,
                     std::string theirOneTimeKey)
        : m_d(new SessionPrivate{
                InboundSessionTag{},
                acc,
                theirIdentityKey,
                theirOneTimeKey})
    {
    }

    Session::~Session() = default;

    Session::Session(const Session &that)
        : m_d(new SessionPrivate(*that.m_d))
    {
    }

    Session::Session(Session &&that)
        : m_d(std::move(that.m_d))
    {
    }

    Session &Session::operator=(const Session &that)
    {
        m_d.reset(new SessionPrivate(*that.m_d));
        return *this;
    }

    Session &Session::operator=(Session &&that)
    {
        m_d = std::move(that.m_d);
        return *this;
    }


    bool Session::matches(std::string message)
    {
        auto res = m_d->checkError(
            olm_matches_inbound_session(m_d->session, message.data(), message.size()));

        // if match, returns 1
        return res == 1;
    }

    bool Session::valid() const
    {
        // maybe a moved-from state, so check m_d first
        return m_d && m_d->valid;
    }

    MaybeString Session::decrypt(int type, std::string message)
    {
        auto msgBuffer = ByteArray(message.begin(), message.end());

        auto size = m_d->checkError(olm_decrypt_max_plaintext_length(
                                        m_d->session, type, msgBuffer.data(), msgBuffer.size()));

        if (size == olm_error()) {
            return NotBut(m_d->error());
        }

        auto plainTextBuffer = ByteArray(size, '\0');

        auto actualSize = m_d->checkError(
            olm_decrypt(m_d->session, type,
                        message.data(), message.size(),
                        plainTextBuffer.data(), plainTextBuffer.size()));

        if (actualSize == olm_error()) {
            return NotBut(m_d->error());
        }

        return std::string(plainTextBuffer.begin(), plainTextBuffer.begin() + actualSize);
    }

    std::pair<int, std::string> Session::encrypt(std::string plainText)
    {
        auto randomData = genRandom(olm_encrypt_random_length(m_d->session));

        auto type = m_d->checkError(olm_encrypt_message_type(m_d->session));

        auto size = m_d->checkError(olm_encrypt_message_length(m_d->session, plainText.size()));

        auto buf = ByteArray(size, '\0');

        auto actualSize = m_d->checkError(
            olm_encrypt(m_d->session, plainText.c_str(), plainText.size(),
                        randomData.data(), randomData.size(),
                        buf.data(), buf.size()));

        if (actualSize != olm_error()) {
            return { type, std::string(buf.begin(), buf.begin() + actualSize) };
        }

        return { -1, "" };
    }

    std::string Session::theirIdentityKey() const
    {
        return m_d->theirIdentityKey;
    }
}
