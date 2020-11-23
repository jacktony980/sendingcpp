/******************************************************************************
 * THIS FILE IS GENERATED - ANY EDITS WILL BE OVERWRITTEN
 */

#pragma once

#include "basejob.hpp"


namespace Kazv {

/*! \brief Gets information about a particular user.
 *
 * Gets information about a particular user.
 * 
 * This API may be restricted to only be called by the user being looked
 * up, or by a server admin. Server-local administrator privileges are not
 * specified in this document.
 */
class GetWhoIsJob : public BaseJob {
public:
  // Inner data structures

/// Gets information about a particular user.
/// 
/// This API may be restricted to only be called by the user being looked
/// up, or by a server admin. Server-local administrator privileges are not
/// specified in this document.
    struct ConnectionInfo
        {
/// Most recently seen IP address of the session.
          std::string ip;
/// Unix timestamp that the session was last active.
          std::optional<std::int_fast64_t> lastSeen;
/// User agent string last seen in the session.
          std::string userAgent;
        
        };

/// Gets information about a particular user.
/// 
/// This API may be restricted to only be called by the user being looked
/// up, or by a server admin. Server-local administrator privileges are not
/// specified in this document.
    struct SessionInfo
        {
/// Information particular connections in the session.
          immer::array<ConnectionInfo> connections;
        
        };

/// Gets information about a particular user.
/// 
/// This API may be restricted to only be called by the user being looked
/// up, or by a server admin. Server-local administrator privileges are not
/// specified in this document.
    struct DeviceInfo
        {
/// A user's sessions (i.e. what they did with an access token from one login).
          immer::array<SessionInfo> sessions;
        
        };



class JobResponse : public Response
{

public:
  JobResponse(Response r);
  bool success() const;

    // Result properties
        
        

    
/// The Matrix user ID of the user.
std::string userId() const;

    
/// Each key is an identifier for one of the user's devices.
immer::map<std::string, DeviceInfo> devices() const;

};
          static constexpr auto needsAuth() {
          return true
            ;
              }


// Construction/destruction

  /*! \brief Gets information about a particular user.
 *
    * \param userId
    *   The user to look up.
    */
    explicit GetWhoIsJob(std::string serverUrl
    , std::string _accessToken
      ,
        std::string userId );


    static BaseJob::Query buildQuery(
    );

      static BaseJob::Body buildBody(std::string userId);

        

      GetWhoIsJob withData(JsonWrap j) &&;
      GetWhoIsJob withData(JsonWrap j) const &;
      };
      using GetWhoIsResponse = GetWhoIsJob::JobResponse;
      } 
      namespace nlohmann
      {
      using namespace Kazv;
      template<>
      struct adl_serializer<GetWhoIsJob::ConnectionInfo> {

  static void from_json(const json &jo, GetWhoIsJob::ConnectionInfo& result)
  {
  
    if (jo.contains("ip"s)) {
      result.ip = jo.at("ip"s);
    }
    if (jo.contains("last_seen"s)) {
      result.lastSeen = jo.at("last_seen"s);
    }
    if (jo.contains("user_agent"s)) {
      result.userAgent = jo.at("user_agent"s);
    }
  
  }
};
      template<>
      struct adl_serializer<GetWhoIsJob::SessionInfo> {

  static void from_json(const json &jo, GetWhoIsJob::SessionInfo& result)
  {
  
    if (jo.contains("connections"s)) {
      result.connections = jo.at("connections"s);
    }
  
  }
};
      template<>
      struct adl_serializer<GetWhoIsJob::DeviceInfo> {

  static void from_json(const json &jo, GetWhoIsJob::DeviceInfo& result)
  {
  
    if (jo.contains("sessions"s)) {
      result.sessions = jo.at("sessions"s);
    }
  
  }
};
    }

    namespace Kazv
    {

} // namespace Kazv
