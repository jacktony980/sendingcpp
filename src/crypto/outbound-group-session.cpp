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


#include "outbound-group-session-p.hpp"

#include <debug.hpp>
#include "time-util.hpp"

namespace Kazv
{
    std::size_t OutboundGroupSessionPrivate::checkError(std::size_t code) const
    {
        if (code == olm_error()) {
            kzo.crypto.warn() << "Olm outbound group session error: "
                              << olm_outbound_group_session_last_error(session) << std::endl;
        }
        return code;
    }

    std::string OutboundGroupSessionPrivate::error() const
    {
        return olm_outbound_group_session_last_error(session);
    }


    OutboundGroupSessionPrivate::OutboundGroupSessionPrivate()
        : sessionData(olm_outbound_group_session_size(), '\0')
        , session(olm_outbound_group_session(sessionData.data()))
    {
        auto randomData = genRandom(olm_init_outbound_group_session_random_length(session));
        auto res = checkError(olm_init_outbound_group_session(session, randomData.data(), randomData.size()));

        if (res != olm_error()) {
            valid = true;
            initialSessionKey = sessionKey();
        }

        creationTime = currentTimeMs();
    }

    OutboundGroupSessionPrivate::OutboundGroupSessionPrivate(const OutboundGroupSessionPrivate &that)
        : sessionData(olm_outbound_group_session_size(), '\0')
        , session(olm_outbound_group_session(sessionData.data()))
        , creationTime(that.creationTime)
        , initialSessionKey(that.initialSessionKey)
    {
        valid = unpickle(that.pickle());
    }

    ByteArray OutboundGroupSessionPrivate::pickle() const
    {
        auto pickleData = ByteArray(olm_pickle_outbound_group_session_length(session), '\0');
        auto key = ByteArray(3, 'x');
        checkError(olm_pickle_outbound_group_session(session,
                                      key.data(), key.size(),
                                      pickleData.data(), pickleData.size()));
        return pickleData;
    }

    bool OutboundGroupSessionPrivate::unpickle(ByteArray pickleData)
    {
        auto key = ByteArray(3, 'x');
        auto res = checkError(olm_unpickle_outbound_group_session(
                                   session,
                                   key.data(), key.size(),
                                   pickleData.data(), pickleData.size()));

        return res != olm_error();
    }


    OutboundGroupSession::OutboundGroupSession()
        : m_d(new OutboundGroupSessionPrivate)
    {
    }

    OutboundGroupSession::~OutboundGroupSession() = default;

    OutboundGroupSession::OutboundGroupSession(const OutboundGroupSession &that)
        : m_d(new OutboundGroupSessionPrivate(*that.m_d))
    {
    }

    OutboundGroupSession::OutboundGroupSession(OutboundGroupSession &&that)
        : m_d(std::move(that.m_d))
    {
    }

    OutboundGroupSession &OutboundGroupSession::operator=(const OutboundGroupSession &that)
    {
        m_d.reset(new OutboundGroupSessionPrivate(*that.m_d));
        return *this;
    }

    OutboundGroupSession &OutboundGroupSession::operator=(OutboundGroupSession &&that)
    {
        m_d = std::move(that.m_d);
        return *this;
    }

    bool OutboundGroupSession::valid() const
    {
        return m_d && m_d->valid;
    }

    std::string OutboundGroupSession::encrypt(std::string plainText)
    {
        auto plain = ByteArray(plainText.begin(), plainText.end());
        auto size = olm_group_encrypt_message_length(m_d->session, plainText.size());
        auto encrypted = ByteArray(size, '\0');


        auto actualSize = m_d->checkError(olm_group_encrypt(
                                              m_d->session,
                                              plain.data(), plain.size(),
                                              encrypted.data(), encrypted.size()));

        return std::string(encrypted.begin(), encrypted.begin() + actualSize);
    }

    std::string OutboundGroupSessionPrivate::sessionKey()
    {
        auto size = olm_outbound_group_session_key_length(session);
        auto keyBuf = ByteArray(size, '\0');
        auto actualSize = checkError(
            olm_outbound_group_session_key(session, keyBuf.data(), keyBuf.size()));

        return std::string(keyBuf.begin(), keyBuf.begin() + actualSize);
    }

    std::string OutboundGroupSession::sessionKey()
    {
        return m_d->sessionKey();
    }

    std::string OutboundGroupSession::initialSessionKey() const
    {
        return m_d->initialSessionKey;
    }

    std::string OutboundGroupSession::sessionId()
    {
        auto size = olm_outbound_group_session_id_length(m_d->session);
        auto idBuf = ByteArray(size, '\0');
        auto actualSize = m_d->checkError(
            olm_outbound_group_session_id(m_d->session, idBuf.data(), idBuf.size()));

        return std::string(idBuf.begin(), idBuf.begin() + actualSize);
    }

    int OutboundGroupSession::messageIndex()
    {
        return olm_outbound_group_session_message_index(m_d->session);
    }

    Timestamp OutboundGroupSession::creationTimeMs() const
    {
        return m_d->creationTime;
    }
}
