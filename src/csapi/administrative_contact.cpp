/******************************************************************************
 * THIS FILE IS GENERATED - ANY EDITS WILL BE OVERWRITTEN
 */

#include <algorithm>

#include "administrative_contact.hpp"

namespace Kazv
{


BaseJob::Query GetAccount3PIDsJob::buildQuery(
)
{
BaseJob::Query _q;

return _q;
}

    BaseJob::Body GetAccount3PIDsJob::buildBody()
      {
      // ignore unused param
      
      
      
              return BaseJob::EmptyBody{};

      };

      

GetAccount3PIDsJob::GetAccount3PIDsJob(
        std::string serverUrl
        , std::string _accessToken
        
        )
      : BaseJob(std::move(serverUrl),
          std::string("/_matrix/client/r0") + "/account/3pid",
          GET,
          _accessToken,
          ReturnType::Json,
            buildBody()
              , buildQuery()
                )
        {
        }

          bool GetAccount3PIDsJob::success(Response r)
          {
            return BaseJob::success(r)
            
              && isBodyJson(r.body)
          ;
          }


    
    immer::array<GetAccount3PIDsJob::ThirdPartyIdentifier> GetAccount3PIDsJob::threepids(Response r)
    {
    if (jsonBody(r).get()
    .contains("threepids"s)) {
    return
    jsonBody(r).get()["threepids"s]
    /*.get<immer::array<ThirdPartyIdentifier>>()*/;}
    else { return immer::array<ThirdPartyIdentifier>(  );}
    }



BaseJob::Query Post3PIDsJob::buildQuery(
)
{
BaseJob::Query _q;

return _q;
}

    BaseJob::Body Post3PIDsJob::buildBody(ThreePidCredentials threePidCreds)
      {
      // ignore unused param
      (void)(threePidCreds);
      
        json _data
        ;
        
            _data["three_pid_creds"s] = threePidCreds;
          
        return BaseJob::JsonBody(_data);
        

      };

      

Post3PIDsJob::Post3PIDsJob(
        std::string serverUrl
        , std::string _accessToken
        ,
        ThreePidCredentials threePidCreds)
      : BaseJob(std::move(serverUrl),
          std::string("/_matrix/client/r0") + "/account/3pid",
          POST,
          _accessToken,
          ReturnType::Json,
            buildBody(threePidCreds)
              , buildQuery()
                )
        {
        }

          bool Post3PIDsJob::success(Response r)
          {
            return BaseJob::success(r)
            
              && isBodyJson(r.body)
          ;
          }




BaseJob::Query Add3PIDJob::buildQuery(
)
{
BaseJob::Query _q;

return _q;
}

    BaseJob::Body Add3PIDJob::buildBody(std::string clientSecret, std::string sid, std::optional<AuthenticationData> auth)
      {
      // ignore unused param
      (void)(clientSecret);(void)(sid);(void)(auth);
      
        json _data
        ;
        
          
            addToJsonIfNeeded(_data, "auth"s, auth);
            _data["client_secret"s] = clientSecret;
          
            _data["sid"s] = sid;
          
        return BaseJob::JsonBody(_data);
        

      };

      

Add3PIDJob::Add3PIDJob(
        std::string serverUrl
        , std::string _accessToken
        ,
        std::string clientSecret, std::string sid, std::optional<AuthenticationData> auth)
      : BaseJob(std::move(serverUrl),
          std::string("/_matrix/client/r0") + "/account/3pid/add",
          POST,
          _accessToken,
          ReturnType::Json,
            buildBody(clientSecret, sid, auth)
              , buildQuery()
                )
        {
        }

          bool Add3PIDJob::success(Response r)
          {
            return BaseJob::success(r)
            
              && isBodyJson(r.body)
          ;
          }




BaseJob::Query Bind3PIDJob::buildQuery(
)
{
BaseJob::Query _q;

return _q;
}

    BaseJob::Body Bind3PIDJob::buildBody(std::string clientSecret, std::string idServer, std::string idAccessToken, std::string sid)
      {
      // ignore unused param
      (void)(clientSecret);(void)(idServer);(void)(idAccessToken);(void)(sid);
      
        json _data
        ;
        
            _data["client_secret"s] = clientSecret;
          
            _data["id_server"s] = idServer;
          
            _data["id_access_token"s] = idAccessToken;
          
            _data["sid"s] = sid;
          
        return BaseJob::JsonBody(_data);
        

      };

      

Bind3PIDJob::Bind3PIDJob(
        std::string serverUrl
        , std::string _accessToken
        ,
        std::string clientSecret, std::string idServer, std::string idAccessToken, std::string sid)
      : BaseJob(std::move(serverUrl),
          std::string("/_matrix/client/r0") + "/account/3pid/bind",
          POST,
          _accessToken,
          ReturnType::Json,
            buildBody(clientSecret, idServer, idAccessToken, sid)
              , buildQuery()
                )
        {
        }

          bool Bind3PIDJob::success(Response r)
          {
            return BaseJob::success(r)
            
              && isBodyJson(r.body)
          ;
          }




BaseJob::Query Delete3pidFromAccountJob::buildQuery(
)
{
BaseJob::Query _q;

return _q;
}

    BaseJob::Body Delete3pidFromAccountJob::buildBody(std::string medium, std::string address, std::string idServer)
      {
      // ignore unused param
      (void)(medium);(void)(address);(void)(idServer);
      
        json _data
        ;
        
          
            addToJsonIfNeeded(_data, "id_server"s, idServer);
            _data["medium"s] = medium;
          
            _data["address"s] = address;
          
        return BaseJob::JsonBody(_data);
        

      };

      

Delete3pidFromAccountJob::Delete3pidFromAccountJob(
        std::string serverUrl
        , std::string _accessToken
        ,
        std::string medium, std::string address, std::string idServer)
      : BaseJob(std::move(serverUrl),
          std::string("/_matrix/client/r0") + "/account/3pid/delete",
          POST,
          _accessToken,
          ReturnType::Json,
            buildBody(medium, address, idServer)
              , buildQuery()
                )
        {
        }

          bool Delete3pidFromAccountJob::success(Response r)
          {
            return BaseJob::success(r)
            
              && isBodyJson(r.body)
            && jsonBody(r).get().contains("id_server_unbind_result"s)
          ;
          }


    
    std::string Delete3pidFromAccountJob::idServerUnbindResult(Response r)
    {
    if (jsonBody(r).get()
    .contains("id_server_unbind_result"s)) {
    return
    jsonBody(r).get()["id_server_unbind_result"s]
    /*.get<std::string>()*/;}
    else { return std::string(  );}
    }



BaseJob::Query Unbind3pidFromAccountJob::buildQuery(
)
{
BaseJob::Query _q;

return _q;
}

    BaseJob::Body Unbind3pidFromAccountJob::buildBody(std::string medium, std::string address, std::string idServer)
      {
      // ignore unused param
      (void)(medium);(void)(address);(void)(idServer);
      
        json _data
        ;
        
          
            addToJsonIfNeeded(_data, "id_server"s, idServer);
            _data["medium"s] = medium;
          
            _data["address"s] = address;
          
        return BaseJob::JsonBody(_data);
        

      };

      

Unbind3pidFromAccountJob::Unbind3pidFromAccountJob(
        std::string serverUrl
        , std::string _accessToken
        ,
        std::string medium, std::string address, std::string idServer)
      : BaseJob(std::move(serverUrl),
          std::string("/_matrix/client/r0") + "/account/3pid/unbind",
          POST,
          _accessToken,
          ReturnType::Json,
            buildBody(medium, address, idServer)
              , buildQuery()
                )
        {
        }

          bool Unbind3pidFromAccountJob::success(Response r)
          {
            return BaseJob::success(r)
            
              && isBodyJson(r.body)
            && jsonBody(r).get().contains("id_server_unbind_result"s)
          ;
          }


    
    std::string Unbind3pidFromAccountJob::idServerUnbindResult(Response r)
    {
    if (jsonBody(r).get()
    .contains("id_server_unbind_result"s)) {
    return
    jsonBody(r).get()["id_server_unbind_result"s]
    /*.get<std::string>()*/;}
    else { return std::string(  );}
    }



BaseJob::Query RequestTokenTo3PIDEmailJob::buildQuery(
)
{
BaseJob::Query _q;

return _q;
}

    BaseJob::Body RequestTokenTo3PIDEmailJob::buildBody(EmailValidationData body)
      {
      // ignore unused param
      (void)(body);
        return 
          BaseJob::JsonBody(body);
      
          

      };

      

RequestTokenTo3PIDEmailJob::RequestTokenTo3PIDEmailJob(
        std::string serverUrl
        
        ,
        EmailValidationData body)
      : BaseJob(std::move(serverUrl),
          std::string("/_matrix/client/r0") + "/account/3pid/email/requestToken",
          POST,
           {} ,
          ReturnType::Json,
            buildBody(body)
              , buildQuery()
                )
        {
        }

          bool RequestTokenTo3PIDEmailJob::success(Response r)
          {
            return BaseJob::success(r)
            
              && isBodyJson(r.body)
          ;
          }




BaseJob::Query RequestTokenTo3PIDMSISDNJob::buildQuery(
)
{
BaseJob::Query _q;

return _q;
}

    BaseJob::Body RequestTokenTo3PIDMSISDNJob::buildBody(MsisdnValidationData body)
      {
      // ignore unused param
      (void)(body);
        return 
          BaseJob::JsonBody(body);
      
          

      };

      

RequestTokenTo3PIDMSISDNJob::RequestTokenTo3PIDMSISDNJob(
        std::string serverUrl
        
        ,
        MsisdnValidationData body)
      : BaseJob(std::move(serverUrl),
          std::string("/_matrix/client/r0") + "/account/3pid/msisdn/requestToken",
          POST,
           {} ,
          ReturnType::Json,
            buildBody(body)
              , buildQuery()
                )
        {
        }

          bool RequestTokenTo3PIDMSISDNJob::success(Response r)
          {
            return BaseJob::success(r)
            
              && isBodyJson(r.body)
          ;
          }


}
