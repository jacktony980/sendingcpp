/******************************************************************************
 * THIS FILE IS GENERATED - ANY EDITS WILL BE OVERWRITTEN
 */

#pragma once

#include "types.hpp"


namespace Kazv {
/// Information about this event which was not sent by the originating homeserver
struct UnsignedData
{       

/// Time in milliseconds since the event was sent.
    std::optional<std::int_fast64_t> age;

/// Optional. The previous ``content`` for this state. This will be present only for state events appearing in the ``timeline``. If this is not a state event, or there is no previous content, this key will be missing.
    JsonWrap prevContent;

/// Optional. The transaction ID set when this message was sent. This key will only be present for message events sent by the device calling this API.
    std::string transactionId;

/// Optional. The event that redacted this event, if any.
    JsonWrap redactedBecause;
};

}
namespace nlohmann
{
using namespace Kazv;
template<>
struct adl_serializer<UnsignedData> {
  static void to_json(json& jo, const UnsignedData &pod)
  {
  
  
    
    addToJsonIfNeeded(jo, "age"s, pod.age);
    
    addToJsonIfNeeded(jo, "prev_content"s, pod.prevContent);
    
    addToJsonIfNeeded(jo, "transaction_id"s, pod.transactionId);
    
    addToJsonIfNeeded(jo, "redacted_because"s, pod.redactedBecause);
  }
  static void from_json(const json &jo, UnsignedData& result)
  {
  
    result.age = jo.at("age"s);
    result.prevContent = jo.at("prev_content"s);
    result.transactionId = jo.at("transaction_id"s);
    result.redactedBecause = jo.at("redacted_because"s);
  
  }
};
    }

    namespace Kazv
    {

struct Event
{       

/// The ID of this event, if applicable.
    std::string eventId;

/// The content of this event. The fields in this object will vary depending on the type of event.
    JsonWrap content;

/// Timestamp in milliseconds on originating homeserver when this event was sent.
    std::optional<std::int_fast64_t> originServerTs;

/// The MXID of the user who sent this event.
    std::string sender;

/// Optional. This key will only be present for state events. A unique key which defines the overwriting semantics for this piece of room state.
    std::string stateKey;

/// The type of event.
    std::string type;

/// Information about this event which was not sent by the originating homeserver
    std::optional<UnsignedData> unsignedData;
};

}
namespace nlohmann
{
using namespace Kazv;
template<>
struct adl_serializer<Event> {
  static void to_json(json& jo, const Event &pod)
  {
  
  
    
    addToJsonIfNeeded(jo, "event_id"s, pod.eventId);
    
    addToJsonIfNeeded(jo, "content"s, pod.content);
    
    addToJsonIfNeeded(jo, "origin_server_ts"s, pod.originServerTs);
    
    addToJsonIfNeeded(jo, "sender"s, pod.sender);
    
    addToJsonIfNeeded(jo, "state_key"s, pod.stateKey);
    
    addToJsonIfNeeded(jo, "type"s, pod.type);
    
    addToJsonIfNeeded(jo, "unsigned"s, pod.unsignedData);
  }
  static void from_json(const json &jo, Event& result)
  {
  
    result.eventId = jo.at("event_id"s);
    result.content = jo.at("content"s);
    result.originServerTs = jo.at("origin_server_ts"s);
    result.sender = jo.at("sender"s);
    result.stateKey = jo.at("state_key"s);
    result.type = jo.at("type"s);
    result.unsignedData = jo.at("unsigned"s);
  
  }
};
    }

    namespace Kazv
    {
} // namespace Kazv
