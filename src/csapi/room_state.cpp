/******************************************************************************
 * THIS FILE IS GENERATED - ANY EDITS WILL BE OVERWRITTEN
 */

#include "room_state.hpp"

namespace Kazv
{
  

    BaseJob::Body SetRoomStateWithKeyJob::buildBody(std::string roomId, std::string eventType, std::string stateKey, JsonWrap body)
      {
      // ignore unused param
      (void)(roomId);(void)(eventType);(void)(stateKey);(void)(body);
        return 
          BaseJob::JsonBody(body);
      
          

      };

SetRoomStateWithKeyJob::SetRoomStateWithKeyJob(
        std::string serverUrl
        , std::string _accessToken
        ,
        std::string roomId, std::string eventType, std::string stateKey, JsonWrap body)
      : BaseJob(std::move(serverUrl),
          std::string("/_matrix/client/r0") + "/rooms/" + roomId + "/state/" + eventType + "/" + stateKey,
          PUT,
          _accessToken,
          ReturnType::Json,
            buildBody(roomId, eventType, stateKey, body)
      )
        {
        
        
          //addExpectedKey("event_id");
        }


    
    std::string SetRoomStateWithKeyJob::eventId(Response r)
    {
    if (jsonBody(r).get()
    .contains("event_id"s)) {
    return
    jsonBody(r).get()["event_id"s]
    /*.get<std::string>()*/;}
    else { return std::string(  );}
    }

}
