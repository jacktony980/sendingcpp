/******************************************************************************
 * THIS FILE IS GENERATED - ANY EDITS WILL BE OVERWRITTEN
 */

#include <algorithm>

#include "profile.hpp"

namespace Kazv
{


BaseJob::Query SetDisplayNameJob::buildQuery(
)
{
BaseJob::Query _q;

return _q;
}

    BaseJob::Body SetDisplayNameJob::buildBody(std::string userId, std::string displayname)
      {
      // ignore unused param
      (void)(userId);(void)(displayname);
      
        json _data
        ;
        
          
            addToJsonIfNeeded(_data, "displayname"s, displayname);
        return BaseJob::JsonBody(_data);
        

      };

      

SetDisplayNameJob::SetDisplayNameJob(
        std::string serverUrl
        , std::string _accessToken
        ,
        std::string userId, std::string displayname)
      : BaseJob(std::move(serverUrl),
          std::string("/_matrix/client/r0") + "/profile/" + userId + "/displayname",
          PUT,
          _accessToken,
          ReturnType::Json,
            buildBody(userId, displayname)
              , buildQuery()
                )
        {
        }

          bool SetDisplayNameJob::success(Response r)
          {
            return BaseJob::success(r)
            
              && isBodyJson(r.body)
          ;
          }




BaseJob::Query GetDisplayNameJob::buildQuery(
)
{
BaseJob::Query _q;

return _q;
}

    BaseJob::Body GetDisplayNameJob::buildBody(std::string userId)
      {
      // ignore unused param
      (void)(userId);
      
      
              return BaseJob::EmptyBody{};

      };

      

GetDisplayNameJob::GetDisplayNameJob(
        std::string serverUrl
        
        ,
        std::string userId)
      : BaseJob(std::move(serverUrl),
          std::string("/_matrix/client/r0") + "/profile/" + userId + "/displayname",
          GET,
           {} ,
          ReturnType::Json,
            buildBody(userId)
              , buildQuery()
                )
        {
        }

          bool GetDisplayNameJob::success(Response r)
          {
            return BaseJob::success(r)
            
              && isBodyJson(r.body)
          ;
          }


    
    std::string GetDisplayNameJob::displayname(Response r)
    {
    if (jsonBody(r).get()
    .contains("displayname"s)) {
    return
    jsonBody(r).get()["displayname"s]
    /*.get<std::string>()*/;}
    else { return std::string(  );}
    }



BaseJob::Query SetAvatarUrlJob::buildQuery(
)
{
BaseJob::Query _q;

return _q;
}

    BaseJob::Body SetAvatarUrlJob::buildBody(std::string userId, std::string avatarUrl)
      {
      // ignore unused param
      (void)(userId);(void)(avatarUrl);
      
        json _data
        ;
        
          
            addToJsonIfNeeded(_data, "avatar_url"s, avatarUrl);
        return BaseJob::JsonBody(_data);
        

      };

      

SetAvatarUrlJob::SetAvatarUrlJob(
        std::string serverUrl
        , std::string _accessToken
        ,
        std::string userId, std::string avatarUrl)
      : BaseJob(std::move(serverUrl),
          std::string("/_matrix/client/r0") + "/profile/" + userId + "/avatar_url",
          PUT,
          _accessToken,
          ReturnType::Json,
            buildBody(userId, avatarUrl)
              , buildQuery()
                )
        {
        }

          bool SetAvatarUrlJob::success(Response r)
          {
            return BaseJob::success(r)
            
              && isBodyJson(r.body)
          ;
          }




BaseJob::Query GetAvatarUrlJob::buildQuery(
)
{
BaseJob::Query _q;

return _q;
}

    BaseJob::Body GetAvatarUrlJob::buildBody(std::string userId)
      {
      // ignore unused param
      (void)(userId);
      
      
              return BaseJob::EmptyBody{};

      };

      

GetAvatarUrlJob::GetAvatarUrlJob(
        std::string serverUrl
        
        ,
        std::string userId)
      : BaseJob(std::move(serverUrl),
          std::string("/_matrix/client/r0") + "/profile/" + userId + "/avatar_url",
          GET,
           {} ,
          ReturnType::Json,
            buildBody(userId)
              , buildQuery()
                )
        {
        }

          bool GetAvatarUrlJob::success(Response r)
          {
            return BaseJob::success(r)
            
              && isBodyJson(r.body)
          ;
          }


    
    std::string GetAvatarUrlJob::avatarUrl(Response r)
    {
    if (jsonBody(r).get()
    .contains("avatar_url"s)) {
    return
    jsonBody(r).get()["avatar_url"s]
    /*.get<std::string>()*/;}
    else { return std::string(  );}
    }



BaseJob::Query GetUserProfileJob::buildQuery(
)
{
BaseJob::Query _q;

return _q;
}

    BaseJob::Body GetUserProfileJob::buildBody(std::string userId)
      {
      // ignore unused param
      (void)(userId);
      
      
              return BaseJob::EmptyBody{};

      };

      

GetUserProfileJob::GetUserProfileJob(
        std::string serverUrl
        
        ,
        std::string userId)
      : BaseJob(std::move(serverUrl),
          std::string("/_matrix/client/r0") + "/profile/" + userId,
          GET,
           {} ,
          ReturnType::Json,
            buildBody(userId)
              , buildQuery()
                )
        {
        }

          bool GetUserProfileJob::success(Response r)
          {
            return BaseJob::success(r)
            
              && isBodyJson(r.body)
          ;
          }


    
    std::string GetUserProfileJob::avatarUrl(Response r)
    {
    if (jsonBody(r).get()
    .contains("avatar_url"s)) {
    return
    jsonBody(r).get()["avatar_url"s]
    /*.get<std::string>()*/;}
    else { return std::string(  );}
    }

    
    std::string GetUserProfileJob::displayname(Response r)
    {
    if (jsonBody(r).get()
    .contains("displayname"s)) {
    return
    jsonBody(r).get()["displayname"s]
    /*.get<std::string>()*/;}
    else { return std::string(  );}
    }

}
