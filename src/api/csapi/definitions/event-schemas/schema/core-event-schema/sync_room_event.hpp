/******************************************************************************
 * THIS FILE IS GENERATED - ANY EDITS WILL BE OVERWRITTEN
 */

#pragma once

#include "types.hpp"
#include "csapi/definitions/event-schemas/schema/core-event-schema/unsigned_prop.hpp"
#include "csapi/definitions/event-schemas/schema/core-event-schema/event.hpp"

namespace Kazv {
/// In addition to the Event fields, Room Events have the following additional fields.
struct SyncRoomEvent : Event
{       

/// The globally unique event identifier.
    std::string eventId;

/// Contains the fully-qualified ID of the user who sent this event.
    std::string sender;

/// Timestamp in milliseconds on originating homeserver when this event was sent.
    std::int_fast64_t originServerTs;

/// In addition to the Event fields, Room Events have the following additional fields.
    std::optional<UnsignedData> unsignedData;
};

}
namespace nlohmann
{
using namespace Kazv;
template<>
struct adl_serializer<SyncRoomEvent> {
  static void to_json(json& jo, const SyncRoomEvent &pod)
  {
    jo = static_cast<const Event &>(pod);
  
    jo["event_id"s] = pod.eventId;
    
    jo["sender"s] = pod.sender;
    
    jo["origin_server_ts"s] = pod.originServerTs;
    
    
    addToJsonIfNeeded(jo, "unsigned"s, pod.unsignedData);
  }
  static void from_json(const json &jo, SyncRoomEvent& result)
  {
    static_cast<Event &>(result) = jo;
    result.eventId = jo.at("event_id"s);
    result.sender = jo.at("sender"s);
    result.originServerTs = jo.at("origin_server_ts"s);
    result.unsignedData = jo.at("unsigned"s);
  
  }
};
    }

    namespace Kazv
    {
} // namespace Kazv
