/******************************************************************************
 * THIS FILE IS GENERATED - ANY EDITS WILL BE OVERWRITTEN
 */

#include <algorithm>

#include "room_send.hpp"

namespace Kazv
{


BaseJob::Query SendMessageJob::buildQuery(
)
{
BaseJob::Query _q;

return _q;
}

    BaseJob::Body SendMessageJob::buildBody(std::string roomId, std::string eventType, std::string txnId, JsonWrap body)
      {
      // ignore unused param
      (void)(roomId);(void)(eventType);(void)(txnId);(void)(body);
        return 
          BaseJob::JsonBody(body);
      
          

      };

      

SendMessageJob::SendMessageJob(
        std::string serverUrl
        , std::string _accessToken
        ,
        std::string roomId, std::string eventType, std::string txnId, JsonWrap body)
      : BaseJob(std::move(serverUrl),
          std::string("/_matrix/client/r0") + "/rooms/" + roomId + "/send/" + eventType + "/" + txnId,
          PUT,
          _accessToken,
          ReturnType::Json,
            buildBody(roomId, eventType, txnId, body)
              , buildQuery()
                )
        {
        }

          bool SendMessageJob::success(Response r)
          {
            return BaseJob::success(r)
            
              && isBodyJson(r.body)
            && jsonBody(r).get().contains("event_id"s)
          ;
          }


    
    std::string SendMessageJob::eventId(Response r)
    {
    if (jsonBody(r).get()
    .contains("event_id"s)) {
    return
    jsonBody(r).get()["event_id"s]
    /*.get<std::string>()*/;}
    else { return std::string(  );}
    }

}
