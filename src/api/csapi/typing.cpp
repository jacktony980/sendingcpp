/******************************************************************************
 * THIS FILE IS GENERATED - ANY EDITS WILL BE OVERWRITTEN
 */

#include <algorithm>

#include "typing.hpp"

namespace Kazv
{


BaseJob::Query SetTypingJob::buildQuery(
)
{
BaseJob::Query _q;

return _q;
}

    BaseJob::Body SetTypingJob::buildBody(std::string userId, std::string roomId, bool typing, std::optional<int> timeout)
      {
      // ignore unused param
      (void)(userId);(void)(roomId);(void)(typing);(void)(timeout);
      
        json _data
        ;
        
            _data["typing"s] = typing;
          
          
            addToJsonIfNeeded(_data, "timeout"s, timeout);
        return BaseJob::JsonBody(_data);
        

      };

      

SetTypingJob::SetTypingJob(
        std::string serverUrl
        , std::string _accessToken
        ,
        std::string userId, std::string roomId, bool typing, std::optional<int> timeout)
      : BaseJob(std::move(serverUrl),
          std::string("/_matrix/client/r0") + "/rooms/" + roomId + "/typing/" + userId,
          PUT,
          _accessToken,
          ReturnType::Json,
            buildBody(userId, roomId, typing, timeout)
              , buildQuery()
                )
        {
        }

          bool SetTypingJob::success(Response r)
          {
            return BaseJob::success(r)
            
              && isBodyJson(r.body)
          ;
          }


}
