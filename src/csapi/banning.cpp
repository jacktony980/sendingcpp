/******************************************************************************
 * THIS FILE IS GENERATED - ANY EDITS WILL BE OVERWRITTEN
 */

#include "banning.hpp"

namespace Kazv
{
  

    BaseJob::Body BanJob::buildBody(std::string roomId, std::string userId, std::string reason)
      {
      // ignore unused param
      (void)(roomId);(void)(userId);(void)(reason);
      
        json _data
        ;
        
            _data["user_id"s] = userId;
          
          
            addToJsonIfNeeded(_data, "reason"s, reason);
        return BaseJob::JsonBody(_data);
        

      };

BanJob::BanJob(
        std::string serverUrl
        , std::string _accessToken
        ,
        std::string roomId, std::string userId, std::string reason)
      : BaseJob(std::move(serverUrl),
          std::string("/_matrix/client/r0") + "/rooms/" + roomId + "/ban",
          POST,
          _accessToken,
          ReturnType::Json,
            buildBody(roomId, userId, reason)
      )
        {
        
        
        }


  

    BaseJob::Body UnbanJob::buildBody(std::string roomId, std::string userId)
      {
      // ignore unused param
      (void)(roomId);(void)(userId);
      
        json _data
        ;
        
            _data["user_id"s] = userId;
          
        return BaseJob::JsonBody(_data);
        

      };

UnbanJob::UnbanJob(
        std::string serverUrl
        , std::string _accessToken
        ,
        std::string roomId, std::string userId)
      : BaseJob(std::move(serverUrl),
          std::string("/_matrix/client/r0") + "/rooms/" + roomId + "/unban",
          POST,
          _accessToken,
          ReturnType::Json,
            buildBody(roomId, userId)
      )
        {
        
        
        }


}
