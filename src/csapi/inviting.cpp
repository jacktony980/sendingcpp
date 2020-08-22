/******************************************************************************
 * THIS FILE IS GENERATED - ANY EDITS WILL BE OVERWRITTEN
 */

#include "inviting.hpp"

namespace Kazv
{
  

    BaseJob::Body InviteUserJob::buildBody(std::string roomId, std::string userId)
      {
      // ignore unused param
      (void)(roomId);(void)(userId);
      
        json _data
        ;
        
            _data["user_id"s] = userId;
          
        return BaseJob::JsonBody(_data);
        

      };

InviteUserJob::InviteUserJob(
        std::string serverUrl
        , std::string _accessToken
        ,
        std::string roomId, std::string userId)
      : BaseJob(std::move(serverUrl),
          std::string("/_matrix/client/r0") + "/rooms/" + roomId + "/invite",
          POST,
          _accessToken,
          ReturnType::Json,
            buildBody(roomId, userId)
      )
        {
        
        
        }


}
