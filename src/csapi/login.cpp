/******************************************************************************
 * THIS FILE IS GENERATED - ANY EDITS WILL BE OVERWRITTEN
 */

#include "login.hpp"

namespace Kazv
{
  

    BaseJob::Body GetLoginFlowsJob::buildBody()
      {
      // ignore unused param
      
      
      
              return BaseJob::EmptyBody{};

      };

GetLoginFlowsJob::GetLoginFlowsJob(
        std::string serverUrl
        
        
        )
      : BaseJob(std::move(serverUrl),
          std::string("/_matrix/client/r0") + "/login",
          GET,
           {} ,
          ReturnType::Json,
            buildBody()
      )
        {
        
        
        }


    
    immer::array<GetLoginFlowsJob::LoginFlow> GetLoginFlowsJob::flows(Response r)
    {
    if (jsonBody(r).get()
    .contains("flows"s)) {
    return
    jsonBody(r).get()["flows"s]
    /*.get<immer::array<LoginFlow>>()*/;}
    else { return immer::array<LoginFlow>(  );}
    }

  

    BaseJob::Body LoginJob::buildBody(std::string type, std::optional<UserIdentifier> identifier, std::string password, std::string token, std::string deviceId, std::string initialDeviceDisplayName)
      {
      // ignore unused param
      (void)(type);(void)(identifier);(void)(password);(void)(token);(void)(deviceId);(void)(initialDeviceDisplayName);
      
        json _data
        ;
        
            _data["type"s] = type;
          
          
            addToJsonIfNeeded(_data, "identifier"s, identifier);
          
            addToJsonIfNeeded(_data, "password"s, password);
          
            addToJsonIfNeeded(_data, "token"s, token);
          
            addToJsonIfNeeded(_data, "device_id"s, deviceId);
          
            addToJsonIfNeeded(_data, "initial_device_display_name"s, initialDeviceDisplayName);
        return BaseJob::JsonBody(_data);
        

      };

LoginJob::LoginJob(
        std::string serverUrl
        
        ,
        std::string type, std::optional<UserIdentifier> identifier, std::string password, std::string token, std::string deviceId, std::string initialDeviceDisplayName)
      : BaseJob(std::move(serverUrl),
          std::string("/_matrix/client/r0") + "/login",
          POST,
           {} ,
          ReturnType::Json,
            buildBody(type, identifier, password, token, deviceId, initialDeviceDisplayName)
      )
        {
        
        
        }


    
    std::string LoginJob::userId(Response r)
    {
    if (jsonBody(r).get()
    .contains("user_id"s)) {
    return
    jsonBody(r).get()["user_id"s]
    /*.get<std::string>()*/;}
    else { return std::string(  );}
    }

    
    std::string LoginJob::accessToken(Response r)
    {
    if (jsonBody(r).get()
    .contains("access_token"s)) {
    return
    jsonBody(r).get()["access_token"s]
    /*.get<std::string>()*/;}
    else { return std::string(  );}
    }

    
    std::string LoginJob::homeServer(Response r)
    {
    if (jsonBody(r).get()
    .contains("home_server"s)) {
    return
    jsonBody(r).get()["home_server"s]
    /*.get<std::string>()*/;}
    else { return std::string(  );}
    }

    
    std::string LoginJob::deviceId(Response r)
    {
    if (jsonBody(r).get()
    .contains("device_id"s)) {
    return
    jsonBody(r).get()["device_id"s]
    /*.get<std::string>()*/;}
    else { return std::string(  );}
    }

    
    std::optional<DiscoveryInformation> LoginJob::wellKnown(Response r)
    {
    if (jsonBody(r).get()
    .contains("well_known"s)) {
    return
    jsonBody(r).get()["well_known"s]
    /*.get<DiscoveryInformation>()*/;}
    else { return std::optional<DiscoveryInformation>(  );}
    }

}
