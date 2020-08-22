/******************************************************************************
 * THIS FILE IS GENERATED - ANY EDITS WILL BE OVERWRITTEN
 */

#include "admin.hpp"

namespace Kazv
{
  

    BaseJob::Body GetWhoIsJob::buildBody(std::string userId)
      {
      // ignore unused param
      (void)(userId);
      
      
              return BaseJob::EmptyBody{};

      };

GetWhoIsJob::GetWhoIsJob(
        std::string serverUrl
        , std::string _accessToken
        ,
        std::string userId)
      : BaseJob(std::move(serverUrl),
          std::string("/_matrix/client/r0") + "/admin/whois/" + userId,
          GET,
          _accessToken,
          ReturnType::Json,
            buildBody(userId)
      )
        {
        
        
        }


    
    std::string GetWhoIsJob::userId(Response r)
    {
    if (jsonBody(r).get()
    .contains("user_id"s)) {
    return
    jsonBody(r).get()["user_id"s]
    /*.get<std::string>()*/;}
    else { return std::string(  );}
    }

    
    immer::map<std::string, GetWhoIsJob::DeviceInfo> GetWhoIsJob::devices(Response r)
    {
    if (jsonBody(r).get()
    .contains("devices"s)) {
    return
    jsonBody(r).get()["devices"s]
    /*.get<immer::map<std::string, DeviceInfo>>()*/;}
    else { return immer::map<std::string, DeviceInfo>(  );}
    }

}
