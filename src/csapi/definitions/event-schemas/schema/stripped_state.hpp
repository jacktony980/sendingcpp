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
  
    result.content = jo.at("content"s);
    result.stateKey = jo.at("state_key"s);
    result.type = jo.at("type"s);
    result.sender = jo.at("sender"s);
  
  }
};
    }

    namespace Kazv
    {
} // namespace Kazv
