/******************************************************************************
 * THIS FILE IS GENERATED - ANY EDITS WILL BE OVERWRITTEN
 */

#pragma once

#include "types.hpp"
#include "csapi/definitions/room_event_batch.hpp"

namespace Kazv {

struct Timeline : RoomEventBatch
{       

/// True if the number of events returned was limited by the ``limit`` on the filter.
    std::optional<bool> limited;

/// A token that can be supplied to the ``from`` parameter of the rooms/{roomId}/messages endpoint.
    std::string prevBatch;
};

}
namespace nlohmann
{
using namespace Kazv;
template<>
struct adl_serializer<Timeline> {
  static void to_json(json& jo, const Timeline &pod)
  {
    jo = static_cast<const RoomEventBatch &>(pod);
  
    
    addToJsonIfNeeded(jo, "limited"s, pod.limited);
    
    addToJsonIfNeeded(jo, "prev_batch"s, pod.prevBatch);
  }
  static void from_json(const json &jo, Timeline& result)
  {
    static_cast<RoomEventBatch &>(result) = jo;
    result.limited = jo.at("limited"s);
    result.prevBatch = jo.at("prev_batch"s);
  
  }
};
    }

    namespace Kazv
    {
} // namespace Kazv
