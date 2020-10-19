/******************************************************************************
 * THIS FILE IS GENERATED - ANY EDITS WILL BE OVERWRITTEN
 */

#include <algorithm>

#include "registration.hpp"

namespace Kazv
{


BaseJob::Query RegisterJob::buildQuery(
std::string kind)
{
BaseJob::Query _q;
  
    addToQueryIfNeeded(_q, "kind"s, kind);
return _q;
}

    BaseJob::Body RegisterJob::buildBody(std::string kind, std::optional<AuthenticationData> auth, std::string username, std::string password, std::string deviceId, std::string initialDeviceDisplayName, std::optional<bool> inhibitLogin)
      {
      // ignore unused param
      (void)(kind);(void)(auth);(void)(username);(void)(password);(void)(deviceId);(void)(initialDeviceDisplayName);(void)(inhibitLogin);
      
        json _data
        ;
        
          
            addToJsonIfNeeded(_data, "auth"s, auth);
          
            addToJsonIfNeeded(_data, "username"s, username);
          
            addToJsonIfNeeded(_data, "password"s, password);
          
            addToJsonIfNeeded(_data, "device_id"s, deviceId);
          
            addToJsonIfNeeded(_data, "initial_device_display_name"s, initialDeviceDisplayName);
          
            addToJsonIfNeeded(_data, "inhibit_login"s, inhibitLogin);
        return BaseJob::JsonBody(_data);
        

      };

      

RegisterJob::RegisterJob(
        std::string serverUrl
        
        ,
        std::string kind, std::optional<AuthenticationData> auth, std::string username, std::string password, std::string deviceId, std::string initialDeviceDisplayName, std::optional<bool> inhibitLogin)
      : BaseJob(std::move(serverUrl),
          std::string("/_matrix/client/r0") + "/register",
          POST,
           {} ,
          ReturnType::Json,
            buildBody(kind, auth, username, password, deviceId, initialDeviceDisplayName, inhibitLogin)
              , buildQuery(kind)
                )
        {
        }

          bool RegisterJob::success(Response r)
          {
            return BaseJob::success(r)
            
              && isBodyJson(r.body)
            && jsonBody(r).get().contains("user_id"s)
          ;
          }


    
    std::string RegisterJob::userId(Response r)
    {
    if (jsonBody(r).get()
    .contains("user_id"s)) {
    return
    jsonBody(r).get()["user_id"s]
    /*.get<std::string>()*/;}
    else { return std::string(  );}
    }

    
    std::string RegisterJob::accessToken(Response r)
    {
    if (jsonBody(r).get()
    .contains("access_token"s)) {
    return
    jsonBody(r).get()["access_token"s]
    /*.get<std::string>()*/;}
    else { return std::string(  );}
    }

    
    std::string RegisterJob::homeServer(Response r)
    {
    if (jsonBody(r).get()
    .contains("home_server"s)) {
    return
    jsonBody(r).get()["home_server"s]
    /*.get<std::string>()*/;}
    else { return std::string(  );}
    }

    
    std::string RegisterJob::deviceId(Response r)
    {
    if (jsonBody(r).get()
    .contains("device_id"s)) {
    return
    jsonBody(r).get()["device_id"s]
    /*.get<std::string>()*/;}
    else { return std::string(  );}
    }



BaseJob::Query RequestTokenToRegisterEmailJob::buildQuery(
)
{
BaseJob::Query _q;

return _q;
}

    BaseJob::Body RequestTokenToRegisterEmailJob::buildBody(EmailValidationData body)
      {
      // ignore unused param
      (void)(body);
        return 
          BaseJob::JsonBody(body);
      
          

      };

      

RequestTokenToRegisterEmailJob::RequestTokenToRegisterEmailJob(
        std::string serverUrl
        
        ,
        EmailValidationData body)
      : BaseJob(std::move(serverUrl),
          std::string("/_matrix/client/r0") + "/register/email/requestToken",
          POST,
           {} ,
          ReturnType::Json,
            buildBody(body)
              , buildQuery()
                )
        {
        }

          bool RequestTokenToRegisterEmailJob::success(Response r)
          {
            return BaseJob::success(r)
            
              && isBodyJson(r.body)
          ;
          }




BaseJob::Query RequestTokenToRegisterMSISDNJob::buildQuery(
)
{
BaseJob::Query _q;

return _q;
}

    BaseJob::Body RequestTokenToRegisterMSISDNJob::buildBody(MsisdnValidationData body)
      {
      // ignore unused param
      (void)(body);
        return 
          BaseJob::JsonBody(body);
      
          

      };

      

RequestTokenToRegisterMSISDNJob::RequestTokenToRegisterMSISDNJob(
        std::string serverUrl
        
        ,
        MsisdnValidationData body)
      : BaseJob(std::move(serverUrl),
          std::string("/_matrix/client/r0") + "/register/msisdn/requestToken",
          POST,
           {} ,
          ReturnType::Json,
            buildBody(body)
              , buildQuery()
                )
        {
        }

          bool RequestTokenToRegisterMSISDNJob::success(Response r)
          {
            return BaseJob::success(r)
            
              && isBodyJson(r.body)
          ;
          }




BaseJob::Query ChangePasswordJob::buildQuery(
)
{
BaseJob::Query _q;

return _q;
}

    BaseJob::Body ChangePasswordJob::buildBody(std::string newPassword, std::optional<bool> logoutDevices, std::optional<AuthenticationData> auth)
      {
      // ignore unused param
      (void)(newPassword);(void)(logoutDevices);(void)(auth);
      
        json _data
        ;
        
            _data["new_password"s] = newPassword;
          
          
            addToJsonIfNeeded(_data, "logout_devices"s, logoutDevices);
          
            addToJsonIfNeeded(_data, "auth"s, auth);
        return BaseJob::JsonBody(_data);
        

      };

      

ChangePasswordJob::ChangePasswordJob(
        std::string serverUrl
        , std::string _accessToken
        ,
        std::string newPassword, std::optional<bool> logoutDevices, std::optional<AuthenticationData> auth)
      : BaseJob(std::move(serverUrl),
          std::string("/_matrix/client/r0") + "/account/password",
          POST,
          _accessToken,
          ReturnType::Json,
            buildBody(newPassword, logoutDevices, auth)
              , buildQuery()
                )
        {
        }

          bool ChangePasswordJob::success(Response r)
          {
            return BaseJob::success(r)
            
              && isBodyJson(r.body)
          ;
          }




BaseJob::Query RequestTokenToResetPasswordEmailJob::buildQuery(
)
{
BaseJob::Query _q;

return _q;
}

    BaseJob::Body RequestTokenToResetPasswordEmailJob::buildBody(EmailValidationData body)
      {
      // ignore unused param
      (void)(body);
        return 
          BaseJob::JsonBody(body);
      
          

      };

      

RequestTokenToResetPasswordEmailJob::RequestTokenToResetPasswordEmailJob(
        std::string serverUrl
        
        ,
        EmailValidationData body)
      : BaseJob(std::move(serverUrl),
          std::string("/_matrix/client/r0") + "/account/password/email/requestToken",
          POST,
           {} ,
          ReturnType::Json,
            buildBody(body)
              , buildQuery()
                )
        {
        }

          bool RequestTokenToResetPasswordEmailJob::success(Response r)
          {
            return BaseJob::success(r)
            
              && isBodyJson(r.body)
          ;
          }




BaseJob::Query RequestTokenToResetPasswordMSISDNJob::buildQuery(
)
{
BaseJob::Query _q;

return _q;
}

    BaseJob::Body RequestTokenToResetPasswordMSISDNJob::buildBody(MsisdnValidationData body)
      {
      // ignore unused param
      (void)(body);
        return 
          BaseJob::JsonBody(body);
      
          

      };

      

RequestTokenToResetPasswordMSISDNJob::RequestTokenToResetPasswordMSISDNJob(
        std::string serverUrl
        
        ,
        MsisdnValidationData body)
      : BaseJob(std::move(serverUrl),
          std::string("/_matrix/client/r0") + "/account/password/msisdn/requestToken",
          POST,
           {} ,
          ReturnType::Json,
            buildBody(body)
              , buildQuery()
                )
        {
        }

          bool RequestTokenToResetPasswordMSISDNJob::success(Response r)
          {
            return BaseJob::success(r)
            
              && isBodyJson(r.body)
          ;
          }




BaseJob::Query DeactivateAccountJob::buildQuery(
)
{
BaseJob::Query _q;

return _q;
}

    BaseJob::Body DeactivateAccountJob::buildBody(std::optional<AuthenticationData> auth, std::string idServer)
      {
      // ignore unused param
      (void)(auth);(void)(idServer);
      
        json _data
        ;
        
          
            addToJsonIfNeeded(_data, "auth"s, auth);
          
            addToJsonIfNeeded(_data, "id_server"s, idServer);
        return BaseJob::JsonBody(_data);
        

      };

      

DeactivateAccountJob::DeactivateAccountJob(
        std::string serverUrl
        , std::string _accessToken
        ,
        std::optional<AuthenticationData> auth, std::string idServer)
      : BaseJob(std::move(serverUrl),
          std::string("/_matrix/client/r0") + "/account/deactivate",
          POST,
          _accessToken,
          ReturnType::Json,
            buildBody(auth, idServer)
              , buildQuery()
                )
        {
        }

          bool DeactivateAccountJob::success(Response r)
          {
            return BaseJob::success(r)
            
              && isBodyJson(r.body)
            && jsonBody(r).get().contains("id_server_unbind_result"s)
          ;
          }


    
    std::string DeactivateAccountJob::idServerUnbindResult(Response r)
    {
    if (jsonBody(r).get()
    .contains("id_server_unbind_result"s)) {
    return
    jsonBody(r).get()["id_server_unbind_result"s]
    /*.get<std::string>()*/;}
    else { return std::string(  );}
    }



BaseJob::Query CheckUsernameAvailabilityJob::buildQuery(
std::string username)
{
BaseJob::Query _q;
    addToQuery(_q, "username"s, username);
  
return _q;
}

    BaseJob::Body CheckUsernameAvailabilityJob::buildBody(std::string username)
      {
      // ignore unused param
      (void)(username);
      
      
              return BaseJob::EmptyBody{};

      };

      

CheckUsernameAvailabilityJob::CheckUsernameAvailabilityJob(
        std::string serverUrl
        
        ,
        std::string username)
      : BaseJob(std::move(serverUrl),
          std::string("/_matrix/client/r0") + "/register/available",
          GET,
           {} ,
          ReturnType::Json,
            buildBody(username)
              , buildQuery(username)
                )
        {
        }

          bool CheckUsernameAvailabilityJob::success(Response r)
          {
            return BaseJob::success(r)
            
              && isBodyJson(r.body)
          ;
          }


    
    std::optional<bool> CheckUsernameAvailabilityJob::available(Response r)
    {
    if (jsonBody(r).get()
    .contains("available"s)) {
    return
    jsonBody(r).get()["available"s]
    /*.get<bool>()*/;}
    else { return std::optional<bool>(  );}
    }

}
