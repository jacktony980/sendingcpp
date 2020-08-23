/******************************************************************************
 * THIS FILE IS GENERATED - ANY EDITS WILL BE OVERWRITTEN
 */

#include <algorithm>

#include "redaction.hpp"

namespace Kazv
{


BaseJob::Query RedactEventJob::buildQuery(
)
{
BaseJob::Query _q;

return _q;
}

    BaseJob::Body RedactEventJob::buildBody(std::string roomId, std::string eventId, std::string txnId, std::string reason)
      {
      // ignore unused param
      (void)(roomId);(void)(eventId);(void)(txnId);(void)(reason);
      
        json _data
        ;
        
          
            addToJsonIfNeeded(_data, "reason"s, reason);
        return BaseJob::JsonBody(_data);
        

      };

      

RedactEventJob::RedactEventJob(
        std::string serverUrl
        , std::string _accessToken
        ,
        std::string roomId, std::string eventId, std::string txnId, std::string reason)
      : BaseJob(std::move(serverUrl),
          std::string("/_matrix/client/r0") + "/rooms/" + roomId + "/redact/" + eventId + "/" + txnId,
          PUT,
          _accessToken,
          ReturnType::Json,
            buildBody(roomId, eventId, txnId, reason)
              , buildQuery()
                )
        {
        }

          bool RedactEventJob::success(Response r)
          {
            return BaseJob::success(r)
            
              && isBodyJson(r.body)
          ;
          }


    
    std::string RedactEventJob::eventId(Response r)
    {
    if (jsonBody(r).get()
    .contains("event_id"s)) {
    return
    jsonBody(r).get()["event_id"s]
    /*.get<std::string>()*/;}
    else { return std::string(  );}
    }

}
