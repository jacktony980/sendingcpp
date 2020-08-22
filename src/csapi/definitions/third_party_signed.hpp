/******************************************************************************
 * THIS FILE IS GENERATED - ANY EDITS WILL BE OVERWRITTEN
 */

#pragma once

#include "types.hpp"


namespace Kazv {
/// A signature of an ``m.third_party_invite`` token to prove that this user
/// owns a third party identity which has been invited to the room.
struct ThirdPartySigned
{       

/// The Matrix ID of the user who issued the invite.
    std::string sender;

/// The Matrix ID of the invitee.
    std::string mxid;

/// The state key of the m.third_party_invite event.
    std::string token;

/// A signatures object containing a signature of the entire signed object.
    immer::map<std::string, immer::map<std::string, std::string>> signatures;
};

}
namespace nlohmann
{
using namespace Kazv;
template<>
struct adl_serializer<ThirdPartySigned> {
  static void to_json(json& jo, const ThirdPartySigned &pod)
  {
  
  
    jo["sender"s] = pod.sender;
    
    jo["mxid"s] = pod.mxid;
    
    jo["token"s] = pod.token;
    
    jo["signatures"s] = pod.signatures;
    
  }
  static void from_json(const json &jo, ThirdPartySigned& result)
  {
  
    result.sender = jo.at("sender"s);
    result.mxid = jo.at("mxid"s);
    result.token = jo.at("token"s);
    result.signatures = jo.at("signatures"s);
  
  }
};
    }

    namespace Kazv
    {
} // namespace Kazv
