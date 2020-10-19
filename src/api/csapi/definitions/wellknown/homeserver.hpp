/******************************************************************************
 * THIS FILE IS GENERATED - ANY EDITS WILL BE OVERWRITTEN
 */

#pragma once

#include "types.hpp"


namespace Kazv {
/// Used by clients to discover homeserver information.
struct HomeserverInformation
{       

/// The base URL for the homeserver for client-server connections.
    std::string baseUrl;
};

}
namespace nlohmann
{
using namespace Kazv;
template<>
struct adl_serializer<HomeserverInformation> {
  static void to_json(json& jo, const HomeserverInformation &pod)
  {
  
  
    jo["base_url"s] = pod.baseUrl;
    
  }
  static void from_json(const json &jo, HomeserverInformation& result)
  {
  
    if (jo.contains("base_url"s)) {
      result.baseUrl = jo.at("base_url"s);
    }
  
  }
};
    }

    namespace Kazv
    {
} // namespace Kazv
