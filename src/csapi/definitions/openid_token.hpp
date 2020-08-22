/******************************************************************************
 * THIS FILE IS GENERATED - ANY EDITS WILL BE OVERWRITTEN
 */

#pragma once

#include "types.hpp"


namespace Kazv {

struct OpenidToken
{       

/// An access token the consumer may use to verify the identity of
/// the person who generated the token. This is given to the federation
/// API ``GET /openid/userinfo`` to verify the user's identity.
    std::string accessToken;

/// The string ``Bearer``.
    std::string tokenType;

/// The homeserver domain the consumer should use when attempting to
/// verify the user's identity.
    std::string matrixServerName;

/// The number of seconds before this token expires and a new one must
/// be generated.
    int expiresIn;
};

}
namespace nlohmann
{
using namespace Kazv;
template<>
struct adl_serializer<OpenidToken> {
  static void to_json(json& jo, const OpenidToken &pod)
  {
  
  
    jo["access_token"s] = pod.accessToken;
    
    jo["token_type"s] = pod.tokenType;
    
    jo["matrix_server_name"s] = pod.matrixServerName;
    
    jo["expires_in"s] = pod.expiresIn;
    
  }
  static void from_json(const json &jo, OpenidToken& result)
  {
  
    result.accessToken = jo.at("access_token"s);
    result.tokenType = jo.at("token_type"s);
    result.matrixServerName = jo.at("matrix_server_name"s);
    result.expiresIn = jo.at("expires_in"s);
  
  }
};
    }

    namespace Kazv
    {
} // namespace Kazv
