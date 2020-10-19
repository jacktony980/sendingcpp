/******************************************************************************
 * THIS FILE IS GENERATED - ANY EDITS WILL BE OVERWRITTEN
 */

#include <algorithm>

#include "report_content.hpp"

namespace Kazv
{


BaseJob::Query ReportContentJob::buildQuery(
)
{
BaseJob::Query _q;

return _q;
}

    BaseJob::Body ReportContentJob::buildBody(std::string roomId, std::string eventId, int score, std::string reason)
      {
      // ignore unused param
      (void)(roomId);(void)(eventId);(void)(score);(void)(reason);
      
        json _data
        ;
        
            _data["score"s] = score;
          
            _data["reason"s] = reason;
          
        return BaseJob::JsonBody(_data);
        

      };

      

ReportContentJob::ReportContentJob(
        std::string serverUrl
        , std::string _accessToken
        ,
        std::string roomId, std::string eventId, int score, std::string reason)
      : BaseJob(std::move(serverUrl),
          std::string("/_matrix/client/r0") + "/rooms/" + roomId + "/report/" + eventId,
          POST,
          _accessToken,
          ReturnType::Json,
            buildBody(roomId, eventId, score, reason)
              , buildQuery()
                )
        {
        }

          bool ReportContentJob::success(Response r)
          {
            return BaseJob::success(r)
            
              && isBodyJson(r.body)
          ;
          }


}
