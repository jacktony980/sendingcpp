/******************************************************************************
 * THIS FILE IS GENERATED - ANY EDITS WILL BE OVERWRITTEN
 */

#pragma once

#include "types.hpp"


namespace Kazv {
/// The basic set of fields all events must have.
struct Event
{       

/// The fields in this object will vary depending on the type of event. When interacting with the REST API, this is the HTTP body.
    JsonWrap content;

/// The type of event. This SHOULD be namespaced similar to Java package naming conventions e.g. 'com.example.subdomain.event.type'
    std::string type;
};

}
namespace nlohmann
{
using namespace Kazv;
template<>
struct adl_serializer<Event> {
  static void to_json(json& jo, const Event &pod)
  {
  
  
    jo["content"s] = pod.content;
    
    jo["type"s] = pod.type;
    
  }
  static void from_json(const json &jo, Event& result)
  {
  
    result.content = jo.at("content"s);
    result.type = jo.at("type"s);
  
  }
};
    }

    namespace Kazv
    {
} // namespace Kazv
