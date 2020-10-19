/******************************************************************************
 * THIS FILE IS GENERATED - ANY EDITS WILL BE OVERWRITTEN
 */

#pragma once

#include "types.hpp"
#include "csapi/definitions/event-schemas/schema/core-event-schema/sync_room_event.hpp"

namespace Kazv {
/// Room Events have the following fields.
struct RoomEvent : SyncRoomEvent
{       

/// The ID of the room associated with this event. Will not be present on events
/// that arrive through ``/sync``, despite being required everywhere else.
    std::string roomId;
};

}
namespace nlohmann
{
using namespace Kazv;
template<>
struct adl_serializer<RoomEvent> {
  static void to_json(json& jo, const RoomEvent &pod)
  {
    jo = static_cast<const SyncRoomEvent &>(pod);
  
    jo["room_id"s] = pod.roomId;
    
  }
  static void from_json(const json &jo, RoomEvent& result)
  {
    static_cast<SyncRoomEvent &>(result) = jo;
    result.roomId = jo.at("room_id"s);
  
  }
};
    }

    namespace Kazv
    {
} // namespace Kazv
