/******************************************************************************
 * THIS FILE IS GENERATED - ANY EDITS WILL BE OVERWRITTEN
 */

#include <algorithm>

#include "device_management.hpp"

namespace Kazv
{


BaseJob::Query GetDevicesJob::buildQuery(
)
{
BaseJob::Query _q;

return _q;
}

    BaseJob::Body GetDevicesJob::buildBody()
      {
      // ignore unused param
      
      
      
              return BaseJob::EmptyBody{};

      };

      

GetDevicesJob::GetDevicesJob(
        std::string serverUrl
        , std::string _accessToken
        
        )
      : BaseJob(std::move(serverUrl),
          std::string("/_matrix/client/r0") + "/devices",
          GET,
          _accessToken,
          ReturnType::Json,
            buildBody()
              , buildQuery()
                )
        {
        }

          bool GetDevicesJob::success(Response r)
          {
            return BaseJob::success(r)
            
              && isBodyJson(r.body)
          ;
          }


    
    immer::array<Device> GetDevicesJob::devices(Response r)
    {
    if (jsonBody(r).get()
    .contains("devices"s)) {
    return
    jsonBody(r).get()["devices"s]
    /*.get<immer::array<Device>>()*/;}
    else { return immer::array<Device>(  );}
    }



BaseJob::Query GetDeviceJob::buildQuery(
)
{
BaseJob::Query _q;

return _q;
}

    BaseJob::Body GetDeviceJob::buildBody(std::string deviceId)
      {
      // ignore unused param
      (void)(deviceId);
      
      
              return BaseJob::EmptyBody{};

      };

      

GetDeviceJob::GetDeviceJob(
        std::string serverUrl
        , std::string _accessToken
        ,
        std::string deviceId)
      : BaseJob(std::move(serverUrl),
          std::string("/_matrix/client/r0") + "/devices/" + deviceId,
          GET,
          _accessToken,
          ReturnType::Json,
            buildBody(deviceId)
              , buildQuery()
                )
        {
        }

          bool GetDeviceJob::success(Response r)
          {
            return BaseJob::success(r)
            
              && isBodyJson(r.body)
          ;
          }




BaseJob::Query UpdateDeviceJob::buildQuery(
)
{
BaseJob::Query _q;

return _q;
}

    BaseJob::Body UpdateDeviceJob::buildBody(std::string deviceId, std::string displayName)
      {
      // ignore unused param
      (void)(deviceId);(void)(displayName);
      
        json _data
        ;
        
          
            addToJsonIfNeeded(_data, "display_name"s, displayName);
        return BaseJob::JsonBody(_data);
        

      };

      

UpdateDeviceJob::UpdateDeviceJob(
        std::string serverUrl
        , std::string _accessToken
        ,
        std::string deviceId, std::string displayName)
      : BaseJob(std::move(serverUrl),
          std::string("/_matrix/client/r0") + "/devices/" + deviceId,
          PUT,
          _accessToken,
          ReturnType::Json,
            buildBody(deviceId, displayName)
              , buildQuery()
                )
        {
        }

          bool UpdateDeviceJob::success(Response r)
          {
            return BaseJob::success(r)
            
              && isBodyJson(r.body)
          ;
          }




BaseJob::Query DeleteDeviceJob::buildQuery(
)
{
BaseJob::Query _q;

return _q;
}

    BaseJob::Body DeleteDeviceJob::buildBody(std::string deviceId, std::optional<AuthenticationData> auth)
      {
      // ignore unused param
      (void)(deviceId);(void)(auth);
      
        json _data
        ;
        
          
            addToJsonIfNeeded(_data, "auth"s, auth);
        return BaseJob::JsonBody(_data);
        

      };

      

DeleteDeviceJob::DeleteDeviceJob(
        std::string serverUrl
        , std::string _accessToken
        ,
        std::string deviceId, std::optional<AuthenticationData> auth)
      : BaseJob(std::move(serverUrl),
          std::string("/_matrix/client/r0") + "/devices/" + deviceId,
          DELETE,
          _accessToken,
          ReturnType::Json,
            buildBody(deviceId, auth)
              , buildQuery()
                )
        {
        }

          bool DeleteDeviceJob::success(Response r)
          {
            return BaseJob::success(r)
            
              && isBodyJson(r.body)
          ;
          }




BaseJob::Query DeleteDevicesJob::buildQuery(
)
{
BaseJob::Query _q;

return _q;
}

    BaseJob::Body DeleteDevicesJob::buildBody(immer::array<std::string> devices, std::optional<AuthenticationData> auth)
      {
      // ignore unused param
      (void)(devices);(void)(auth);
      
        json _data
        ;
        
            _data["devices"s] = devices;
          
          
            addToJsonIfNeeded(_data, "auth"s, auth);
        return BaseJob::JsonBody(_data);
        

      };

      

DeleteDevicesJob::DeleteDevicesJob(
        std::string serverUrl
        , std::string _accessToken
        ,
        immer::array<std::string> devices, std::optional<AuthenticationData> auth)
      : BaseJob(std::move(serverUrl),
          std::string("/_matrix/client/r0") + "/delete_devices",
          POST,
          _accessToken,
          ReturnType::Json,
            buildBody(devices, auth)
              , buildQuery()
                )
        {
        }

          bool DeleteDevicesJob::success(Response r)
          {
            return BaseJob::success(r)
            
              && isBodyJson(r.body)
          ;
          }


}
