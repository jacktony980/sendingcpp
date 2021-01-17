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


#include "inbound-group-session-p.hpp"

#include <debug.hpp>

namespace Kazv
{
    std::size_t InboundGroupSessionPrivate::checkError(std::size_t code) const
    {
        if (code == olm_error()) {
            kzo.crypto.warn() << "Olm inbound group session error: "
                              << olm_inbound_group_session_last_error(session) << std::endl;
        }
        return code;
    }

    std::string InboundGroupSessionPrivate::error() const
    {
        return olm_inbound_group_session_last_error(session);
    }


    InboundGroupSessionPrivate::InboundGroupSessionPrivate()
        : sessionData(olm_inbound_group_session_size(), '\0')
        , session(olm_inbound_group_session(sessionData.data()))
    {
    }

    InboundGroupSessionPrivate::InboundGroupSessionPrivate(std::string sessionKey, std::string ed25519Key)
        : InboundGroupSessionPrivate()
    {
        this->ed25519Key = ed25519Key;

        auto keyBuf = ByteArray(sessionKey.begin(), sessionKey.end());

        auto res = checkError(olm_init_inbound_group_session(session, keyBuf.data(), keyBuf.size()));
        if (res != olm_error()) {
            valid = true;
        }
    }

    InboundGroupSessionPrivate::InboundGroupSessionPrivate(const InboundGroupSessionPrivate &that)
        : InboundGroupSessionPrivate()
    {
        ed25519Key = that.ed25519Key;
        valid = unpickle(that.pickle());
    }

    ByteArray InboundGroupSessionPrivate::pickle() const
    {
        auto pickleData = ByteArray(olm_pickle_inbound_group_session_length(session), '\0');
        auto key = ByteArray(3, 'x');
        checkError(olm_pickle_inbound_group_session(session,
                                      key.data(), key.size(),
                                      pickleData.data(), pickleData.size()));
        return pickleData;
    }

    bool InboundGroupSessionPrivate::unpickle(ByteArray pickleData)
    {
        auto key = ByteArray(3, 'x');
        auto res = checkError(olm_unpickle_inbound_group_session(
                                   session,
                                   key.data(), key.size(),
                                   pickleData.data(), pickleData.size()));

        return res != olm_error();
    }


    InboundGroupSession::InboundGroupSession()
        : m_d(new InboundGroupSessionPrivate)
    {
    }

    InboundGroupSession::InboundGroupSession(std::string sessionKey, std::string ed25519Key)
        : m_d(new InboundGroupSessionPrivate(std::move(sessionKey), std::move(ed25519Key)))
    {
    }

    InboundGroupSession::~InboundGroupSession() = default;

    InboundGroupSession::InboundGroupSession(const InboundGroupSession &that)
        : m_d(new InboundGroupSessionPrivate(*that.m_d))
    {
    }

    InboundGroupSession::InboundGroupSession(InboundGroupSession &&that)
        : m_d(std::move(that.m_d))
    {
    }

    InboundGroupSession &InboundGroupSession::operator=(const InboundGroupSession &that)
    {
        m_d.reset(new InboundGroupSessionPrivate(*that.m_d));
        return *this;
    }

    InboundGroupSession &InboundGroupSession::operator=(InboundGroupSession &&that)
    {
        m_d = std::move(that.m_d);
        return *this;
    }

    bool InboundGroupSession::valid() const
    {
        return m_d && m_d->valid;
    }

    MaybeString InboundGroupSession::decrypt(std::string message, std::string eventId, std::int_fast64_t originServerTs)
    {
        ByteArray msgBuffer(message.begin(), message.end());
        ByteArray msgBuffer2 = msgBuffer;

        auto size = m_d->checkError(olm_group_decrypt_max_plaintext_length(
                                        m_d->session,
                                        msgBuffer.data(), msgBuffer.size()));
        if (size == olm_error()) {
            return NotBut(m_d->error());
        }

        auto plainText = ByteArray(size, '\0');
        std::uint32_t messageIndex;

        auto actualSize = m_d->checkError(olm_group_decrypt(
                                              m_d->session,
                                              msgBuffer2.data(), msgBuffer2.size(),
                                              plainText.data(), plainText.size(),
                                              &messageIndex));

        if (actualSize == olm_error()) {
            return NotBut(m_d->error());
        }

        // Check for possible replay attack
        auto keyForThisMsg = KeyOfDecryptedEvent{eventId, originServerTs};

        if (! m_d->decryptedEvents.find(messageIndex)) {
            m_d->decryptedEvents = std::move(m_d->decryptedEvents)
                .set(messageIndex, keyForThisMsg);
        } else { // already decrypted in the past
            auto key = m_d->decryptedEvents.at(messageIndex);
            if (key != keyForThisMsg) {
                return NotBut("This message has been decrypted in the past, but eventId or originServerTs does not match");
            }
        }

        return std::string(plainText.begin(), plainText.begin() + actualSize);
    }

    std::string InboundGroupSession::ed25519Key() const
    {
        return m_d->ed25519Key;
    }
}
