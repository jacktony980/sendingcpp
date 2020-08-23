/******************************************************************************
 * THIS FILE IS GENERATED - ANY EDITS WILL BE OVERWRITTEN
 */

#pragma once

#include "types.hpp"
#include "csapi/definitions/wellknown/identity_server.hpp"
#include "csapi/definitions/wellknown/homeserver.hpp"

namespace Kazv {
/// Used by clients to determine the homeserver, identity server, and other
/// optional components they should be interacting with.
struct DiscoveryInformation
{       

/// Used by clients to determine the homeserver, identity server, and other
/// optional components they should be interacting with.
    HomeserverInformation homeserver;

/// Used by clients to determine the homeserver, identity server, and other
/// optional components they should be interacting with.
    std::optional<IdentityServerInformation> identityServer;

/// Application-dependent keys using Java package naming convention.
    immer::map<std::string, JsonWrap> additionalProperties;
};

}
namespace nlohmann
{
using namespace Kazv;
template<>
struct adl_serializer<DiscoveryInformation> {
  static void to_json(json& jo, const DiscoveryInformation &pod)
  {
  
    addPropertyMapToJson(jo, pod.additionalProperties);
    jo["m.homeserver"s] = pod.homeserver;
    
    
    addToJsonIfNeeded(jo, "m.identity_server"s, pod.identityServer);
  }
  static void from_json(const json &jo, DiscoveryInformation& result)
  {
  
    if (jo.contains("m.homeserver"s)) {
      result.homeserver = jo.at("m.homeserver"s);
    }
    if (jo.contains("m.identity_server"s)) {
      result.identityServer = jo.at("m.identity_server"s);
    }
    result.additionalProperties = jo;
  }
};
    }

    namespace Kazv
    {
} // namespace Kazv
