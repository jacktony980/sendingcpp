
#pragma once
#include <variant>
#include "types.hpp"

namespace Kazv
{
    struct LoginSuccessful {};
    inline bool operator==(LoginSuccessful, LoginSuccessful)
    {
        return true;
    }

    struct ReceivingPresenceEvent { JsonWrap event; };
    inline bool operator==(ReceivingPresenceEvent a, ReceivingPresenceEvent b)
    {
        return a.event == b.event;
    }

    struct ReceivingAccountDataEvent { JsonWrap event; };
    inline bool operator==(ReceivingAccountDataEvent a, ReceivingAccountDataEvent b)
    {
        return a.event == b.event;
    }

    struct ReceivingRoomTimelineEvent {
        JsonWrap event;
        std::string roomId;
    };

    inline bool operator==(ReceivingRoomTimelineEvent a, ReceivingRoomTimelineEvent b)
    {
        return a.event == b.event && a.roomId == b.roomId;
    }


    using KazvEvent = std::variant<
        LoginSuccessful,
        ReceivingPresenceEvent,
        ReceivingAccountDataEvent,
        ReceivingRoomTimelineEvent
        >;

    class EventInterface
    {
    public:
        virtual ~EventInterface() = default;
        virtual void emit(KazvEvent e) = 0;
    };
}
