/******************************************************************************
 * THIS FILE IS GENERATED - ANY EDITS WILL BE OVERWRITTEN
 */

#pragma once

#include "types.hpp"


namespace Kazv {
/// Used by clients to submit authentication information to the interactive-authentication API
struct AuthenticationData
{       

/// The login type that the client is attempting to complete.
    std::string type;

/// The value of the session key given by the homeserver.
    std::string session;

/// Keys dependent on the login type
    immer::map<std::string, JsonWrap> authInfo;
};

}
namespace nlohmann
{
using namespace Kazv;
template<>
struct adl_serializer<AuthenticationData> {
  static void to_json(json& jo, const AuthenticationData &pod)
  {
  
    addPropertyMapToJson(jo, pod.authInfo);
    jo["type"s] = pod.type;
    
    
    addToJsonIfNeeded(jo, "session"s, pod.session);
  }
  static void from_json(const json &jo, AuthenticationData& result)
  {
  
    result.type = jo.at("type"s);
    result.session = jo.at("session"s);
    result.authInfo = jo;
  }
};
    }

    namespace Kazv
    {
} // namespace Kazv
