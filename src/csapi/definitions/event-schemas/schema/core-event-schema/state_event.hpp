/******************************************************************************
 * THIS FILE IS GENERATED - ANY EDITS WILL BE OVERWRITTEN
 */

#pragma once

#include "types.hpp"
#include "csapi/definitions/event-schemas/schema/core-event-schema/sync_state_event.hpp"
#include "csapi/definitions/event-schemas/schema/core-event-schema/room_event.hpp"

namespace Kazv {
/// State Events have the following fields.
struct StateEvent : RoomEvent, SyncStateEvent
{       
};

}
namespace nlohmann
{
using namespace Kazv;
template<>
struct adl_serializer<StateEvent> {
  static void to_json(json& jo, const StateEvent &pod)
  {
    jo = static_cast<const RoomEvent &>(pod);
    jo = static_cast<const SyncStateEvent &>(pod);
  
  
  }
  static void from_json(const json &jo, StateEvent& result)
  {
    static_cast<RoomEvent &>(result) = jo;
    static_cast<SyncStateEvent &>(result) = jo;
  
  
  }
};
    }

    namespace Kazv
    {
} // namespace Kazv
