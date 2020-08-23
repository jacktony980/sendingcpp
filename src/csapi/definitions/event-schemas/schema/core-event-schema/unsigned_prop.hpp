/******************************************************************************
 * THIS FILE IS GENERATED - ANY EDITS WILL BE OVERWRITTEN
 */

#pragma once

#include "types.hpp"


namespace Kazv {
/// Contains optional extra information about the event.
struct UnsignedData
{       

/// The time in milliseconds that has elapsed since the event was sent. This field is generated by the local homeserver, and may be incorrect if the local time on at least one of the two servers is out of sync, which can cause the age to either be negative or greater than it actually is.
    std::optional<int> age;

/// The event that redacted this event, if any.
    JsonWrap redactedBecause;

/// The client-supplied transaction ID, for example, provided via
/// ``PUT /_matrix/client/r0/rooms/{roomId}/send/{eventType}/{txnId}``,
/// if the client being given the event is the same one which sent it.
    std::string transactionId;
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
    
    addToJsonIfNeeded(jo, "redacted_because"s, pod.redactedBecause);
    
    addToJsonIfNeeded(jo, "transaction_id"s, pod.transactionId);
  }
  static void from_json(const json &jo, UnsignedData& result)
  {
  
    if (jo.contains("age"s)) {
      result.age = jo.at("age"s);
    }
    if (jo.contains("redacted_because"s)) {
      result.redactedBecause = jo.at("redacted_because"s);
    }
    if (jo.contains("transaction_id"s)) {
      result.transactionId = jo.at("transaction_id"s);
    }
  
  }
};
    }

    namespace Kazv
    {
} // namespace Kazv
