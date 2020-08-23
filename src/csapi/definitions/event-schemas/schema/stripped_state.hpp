/******************************************************************************
 * THIS FILE IS GENERATED - ANY EDITS WILL BE OVERWRITTEN
 */

#pragma once

#include "types.hpp"


namespace Kazv {
/// A stripped down state event, with only the ``type``, ``state_key``,
/// ``sender``, and ``content`` keys.
struct StrippedState
{       

/// The ``content`` for the event.
    JsonWrap content;

/// The ``state_key`` for the event.
    std::string stateKey;

/// The ``type`` for the event.
    std::string type;

/// The ``sender`` for the event.
    std::string sender;
};

}
namespace nlohmann
{
using namespace Kazv;
template<>
struct adl_serializer<StrippedState> {
  static void to_json(json& jo, const StrippedState &pod)
  {
  
  
    jo["content"s] = pod.content;
    
    jo["state_key"s] = pod.stateKey;
    
    jo["type"s] = pod.type;
    
    jo["sender"s] = pod.sender;
    
  }
  static void from_json(const json &jo, StrippedState& result)
  {
  
    if (jo.contains("content"s)) {
      result.content = jo.at("content"s);
    }
    if (jo.contains("state_key"s)) {
      result.stateKey = jo.at("state_key"s);
    }
    if (jo.contains("type"s)) {
      result.type = jo.at("type"s);
    }
    if (jo.contains("sender"s)) {
      result.sender = jo.at("sender"s);
    }
  
  }
};
    }

    namespace Kazv
    {
} // namespace Kazv
