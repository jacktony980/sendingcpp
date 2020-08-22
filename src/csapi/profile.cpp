/******************************************************************************
 * THIS FILE IS GENERATED - ANY EDITS WILL BE OVERWRITTEN
 */

#include "profile.hpp"

namespace Kazv
{
  

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
      )
        {
        
        
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
      )
        {
        
        
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
      )
        {
        
        
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
      )
        {
        
        
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
      )
        {
        
        
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
