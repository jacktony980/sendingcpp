/******************************************************************************
 * THIS FILE IS GENERATED - ANY EDITS WILL BE OVERWRITTEN
 */

#include <algorithm>

#include "inviting.hpp"

namespace Kazv
{


BaseJob::Query InviteUserJob::buildQuery(
)
{
BaseJob::Query _q;

return _q;
}

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
              , buildQuery()
                )
        {
        }

          bool InviteUserJob::success(Response r)
          {
            return BaseJob::success(r)
            
              && isBodyJson(r.body)
          ;
          }


}
