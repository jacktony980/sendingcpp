/******************************************************************************
 * THIS FILE IS GENERATED - ANY EDITS WILL BE OVERWRITTEN
 */

#pragma once

#include "basejob.hpp"


namespace Kazv {

/*! \brief Gets information about the server's capabilities.
 *
 * Gets information about the server's supported feature set
 * and other relevant capabilities.
 */
class GetCapabilitiesJob : public BaseJob {
public:
  // Inner data structures

/// Capability to indicate if the user can change their password.
    struct ChangePasswordCapability
        {
/// True if the user can change their password, false otherwise.
          bool enabled;
        
        };

/// The room versions the server supports.
    struct RoomVersionsCapability
        {
/// The default room version the server is using for new rooms.
          std::string defaultVersion;
/// A detailed description of the room versions the server supports.
          immer::map<std::string, std::string> available;
        
        };

/// The custom capabilities the server supports, using the
/// Java package naming convention.
    struct Capabilities
        {
/// Capability to indicate if the user can change their password.
          std::optional<ChangePasswordCapability> changePassword;
/// The room versions the server supports.
          std::optional<RoomVersionsCapability> roomVersions;
/// The custom capabilities the server supports, using the
/// Java package naming convention.
          immer::map<std::string, JsonWrap> additionalProperties;
        };


// Construction/destruction

    /// Gets information about the server's capabilities.
    explicit GetCapabilitiesJob(std::string serverUrl
    , std::string _accessToken
      
        );


    // Result properties
        
        

    
/// The custom capabilities the server supports, using the
/// Java package naming convention.
static Capabilities capabilities(Response r);

    static BaseJob::Query buildQuery(
    );

      static BaseJob::Body buildBody();

        static bool success(Response r);
        
      };

      } 
      namespace nlohmann
      {
      using namespace Kazv;
      template<>
      struct adl_serializer<GetCapabilitiesJob::ChangePasswordCapability> {

  static void from_json(const json &jo, GetCapabilitiesJob::ChangePasswordCapability& result)
  {
  
    if (jo.contains("enabled"s)) {
      result.enabled = jo.at("enabled"s);
    }
  
  }
};
      template<>
      struct adl_serializer<GetCapabilitiesJob::RoomVersionsCapability> {

  static void from_json(const json &jo, GetCapabilitiesJob::RoomVersionsCapability& result)
  {
  
    if (jo.contains("default"s)) {
      result.defaultVersion = jo.at("default"s);
    }
    if (jo.contains("available"s)) {
      result.available = jo.at("available"s);
    }
  
  }
};
      template<>
      struct adl_serializer<GetCapabilitiesJob::Capabilities> {

  static void from_json(const json &jo, GetCapabilitiesJob::Capabilities& result)
  {
  
    if (jo.contains("m.change_password"s)) {
      result.changePassword = jo.at("m.change_password"s);
    }
    if (jo.contains("m.room_versions"s)) {
      result.roomVersions = jo.at("m.room_versions"s);
    }
    result.additionalProperties = jo;
  }
};
    }

    namespace Kazv
    {

} // namespace Kazv
