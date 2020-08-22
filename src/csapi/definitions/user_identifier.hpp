/******************************************************************************
 * THIS FILE IS GENERATED - ANY EDITS WILL BE OVERWRITTEN
 */

#pragma once

#include "types.hpp"


namespace Kazv {
/// Identification information for a user
struct UserIdentifier
{       

/// The type of identification.  See `Identifier types`_ for supported values and additional property descriptions.
    std::string type;

/// Identification information for a user
    JsonWrap additionalProperties;
};

}
namespace nlohmann
{
using namespace Kazv;
template<>
struct adl_serializer<UserIdentifier> {
  static void to_json(json& jo, const UserIdentifier &pod)
  {
  
    addPropertyMapToJson(jo, pod.additionalProperties);
    jo["type"s] = pod.type;
    
  }
  static void from_json(const json &jo, UserIdentifier& result)
  {
  
    result.type = jo.at("type"s);
    result.additionalProperties = jo;
  }
};
    }

    namespace Kazv
    {
} // namespace Kazv
