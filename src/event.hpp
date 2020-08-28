
#pragma once

#include "jsonwrap.hpp"
#include <string>

namespace Kazv
{
    class Event
    {
    public:
        static const JsonWrap notYetDecryptedEvent;

        enum DecryptionStatus {
            NotDecrypted,
            Decrypted
        };

        Event();

        Event(JsonWrap j);

        static Event fromSync(Event e, std::string roomId);

        /// returns the id of this event
        std::string id() const;

        std::string sender() const;

        std::string originServerTs() const;

        std::string type() const;

        JsonWrap content() const;

        /// returns the decrypted json
        JsonWrap raw() const;

        /// returns the original json we fetched, probably encrypted.
        JsonWrap originalJson() const;

        JsonWrap decryptedJson() const;

        bool encrypted() const;

        bool decrypted() const;

        /// internal. only to be called from inside the client.
        Event setDecryptedJson(JsonWrap decryptedJson, DecryptionStatus decrypted) const;

    private:
        JsonWrap m_json;
        JsonWrap m_decryptedJson;
        DecryptionStatus m_decrypted{NotDecrypted};
        bool m_encrypted{false};
    };

    bool operator==(Event a, Event b);
}

namespace nlohmann
{
    template <>
    struct adl_serializer<Kazv::Event> {
        static void to_json(json& j, Kazv::Event w) {
            j = w.originalJson();
        }

        static void from_json(const json& j, Kazv::Event &w) {
            w = Kazv::Event(Kazv::JsonWrap(j));
        }
    };
}
