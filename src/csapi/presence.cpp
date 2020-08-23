/******************************************************************************
 * THIS FILE IS GENERATED - ANY EDITS WILL BE OVERWRITTEN
 */

#include <algorithm>

#include "presence.hpp"

namespace Kazv
{


BaseJob::Query SetPresenceJob::buildQuery(
)
{
BaseJob::Query _q;

return _q;
}

    BaseJob::Body SetPresenceJob::buildBody(std::string userId, std::string presence, std::string statusMsg)
      {
      // ignore unused param
      (void)(userId);(void)(presence);(void)(statusMsg);
      
        json _data
        ;
        
            _data["presence"s] = presence;
          
          
            addToJsonIfNeeded(_data, "status_msg"s, statusMsg);
        return BaseJob::JsonBody(_data);
        

      };

      

SetPresenceJob::SetPresenceJob(
        std::string serverUrl
        , std::string _accessToken
        ,
        std::string userId, std::string presence, std::string statusMsg)
      : BaseJob(std::move(serverUrl),
          std::string("/_matrix/client/r0") + "/presence/" + userId + "/status",
          PUT,
          _accessToken,
          ReturnType::Json,
            buildBody(userId, presence, statusMsg)
              , buildQuery()
                )
        {
        }

          bool SetPresenceJob::success(Response r)
          {
            return BaseJob::success(r)
            
              && isBodyJson(r.body)
          ;
          }




BaseJob::Query GetPresenceJob::buildQuery(
)
{
BaseJob::Query _q;

return _q;
}

    BaseJob::Body GetPresenceJob::buildBody(std::string userId)
      {
      // ignore unused param
      (void)(userId);
      
      
              return BaseJob::EmptyBody{};

      };

      

GetPresenceJob::GetPresenceJob(
        std::string serverUrl
        , std::string _accessToken
        ,
        std::string userId)
      : BaseJob(std::move(serverUrl),
          std::string("/_matrix/client/r0") + "/presence/" + userId + "/status",
          GET,
          _accessToken,
          ReturnType::Json,
            buildBody(userId)
              , buildQuery()
                )
        {
        }

          bool GetPresenceJob::success(Response r)
          {
            return BaseJob::success(r)
            
              && isBodyJson(r.body)
            && jsonBody(r).get().contains("presence"s)
          ;
          }


    
    std::string GetPresenceJob::presence(Response r)
    {
    if (jsonBody(r).get()
    .contains("presence"s)) {
    return
    jsonBody(r).get()["presence"s]
    /*.get<std::string>()*/;}
    else { return std::string(  );}
    }

    
    std::optional<int> GetPresenceJob::lastActiveAgo(Response r)
    {
    if (jsonBody(r).get()
    .contains("last_active_ago"s)) {
    return
    jsonBody(r).get()["last_active_ago"s]
    /*.get<int>()*/;}
    else { return std::optional<int>(  );}
    }

    
    Variant GetPresenceJob::statusMsg(Response r)
    {
    if (jsonBody(r).get()
    .contains("status_msg"s)) {
    return
    jsonBody(r).get()["status_msg"s]
    /*.get<Variant>()*/;}
    else { return Variant(  );}
    }

    
    std::optional<bool> GetPresenceJob::currentlyActive(Response r)
    {
    if (jsonBody(r).get()
    .contains("currently_active"s)) {
    return
    jsonBody(r).get()["currently_active"s]
    /*.get<bool>()*/;}
    else { return std::optional<bool>(  );}
    }

}
