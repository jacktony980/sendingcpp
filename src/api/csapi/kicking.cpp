/******************************************************************************
 * THIS FILE IS GENERATED - ANY EDITS WILL BE OVERWRITTEN
 */

#include <algorithm>

#include "kicking.hpp"

namespace Kazv
{


BaseJob::Query KickJob::buildQuery(
)
{
BaseJob::Query _q;

return _q;
}

    BaseJob::Body KickJob::buildBody(std::string roomId, std::string userId, std::string reason)
      {
      // ignore unused param
      (void)(roomId);(void)(userId);(void)(reason);
      
        json _data
        ;
        
            _data["user_id"s] = userId;
          
          
            addToJsonIfNeeded(_data, "reason"s, reason);
        return BaseJob::JsonBody(_data);
        

      };

      

KickJob::KickJob(
        std::string serverUrl
        , std::string _accessToken
        ,
        std::string roomId, std::string userId, std::string reason)
      : BaseJob(std::move(serverUrl),
          std::string("/_matrix/client/r0") + "/rooms/" + roomId + "/kick",
          POST,
          _accessToken,
          ReturnType::Json,
            buildBody(roomId, userId, reason)
              , buildQuery()
                )
        {
        }

          bool KickJob::success(Response r)
          {
            return BaseJob::success(r)
            
              && isBodyJson(r.body)
          ;
          }


}
