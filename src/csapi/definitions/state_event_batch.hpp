/******************************************************************************
 * THIS FILE IS GENERATED - ANY EDITS WILL BE OVERWRITTEN
 */

#pragma once

#include "types.hpp"


namespace Kazv {

struct StateEventBatch
{       

/// List of events.
    EventList events;
};

}
namespace nlohmann
{
using namespace Kazv;
template<>
struct adl_serializer<StateEventBatch> {
  static void to_json(json& jo, const StateEventBatch &pod)
  {
  
  
    
    addToJsonIfNeeded(jo, "events"s, pod.events);
  }
  static void from_json(const json &jo, StateEventBatch& result)
  {
  
    if (jo.contains("events"s)) {
      result.events = jo.at("events"s);
    }
  
  }
};
    }

    namespace Kazv
    {
} // namespace Kazv
