/******************************************************************************
 * THIS FILE IS GENERATED - ANY EDITS WILL BE OVERWRITTEN
 */

#pragma once

#include "types.hpp"
#include "csapi/definitions/event-schemas/schema/core-event-schema/sync_room_event.hpp"

namespace Kazv {
/// In addition to the Room Event fields, State Events have the following additional fields.
struct SyncStateEvent : SyncRoomEvent
{       

/// Optional. The previous ``content`` for this event. If there is no previous content, this key will be missing.
    JsonWrap prevContent;

/// A unique key which defines the overwriting semantics for this piece of room state. This value is often a zero-length string. The presence of this key makes this event a State Event.
/// State keys starting with an ``@`` are reserved for referencing user IDs, such as room members. With the exception of a few events, state events set with a given user's ID as the state key MUST only be set by that user.
    std::string stateKey;
};

}
namespace nlohmann
{
using namespace Kazv;
template<>
struct adl_serializer<SyncStateEvent> {
  static void to_json(json& jo, const SyncStateEvent &pod)
  {
    jo = static_cast<const SyncRoomEvent &>(pod);
  
    
    addToJsonIfNeeded(jo, "prev_content"s, pod.prevContent);
    jo["state_key"s] = pod.stateKey;
    
  }
  static void from_json(const json &jo, SyncStateEvent& result)
  {
    static_cast<SyncRoomEvent &>(result) = jo;
    result.prevContent = jo.at("prev_content"s);
    result.stateKey = jo.at("state_key"s);
  
  }
};
    }

    namespace Kazv
    {
} // namespace Kazv
