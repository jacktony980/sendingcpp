/******************************************************************************
 * THIS FILE IS GENERATED - ANY EDITS WILL BE OVERWRITTEN
 */

#include <algorithm>

#include "peeking_events.hpp"

namespace Kazv
{


BaseJob::Query PeekEventsJob::buildQuery(
std::string from, std::optional<int> timeout, std::string roomId)
{
BaseJob::Query _q;
  
    addToQueryIfNeeded(_q, "from"s, from);
  
    addToQueryIfNeeded(_q, "timeout"s, timeout);
  
    addToQueryIfNeeded(_q, "room_id"s, roomId);
return _q;
}

    BaseJob::Body PeekEventsJob::buildBody(std::string from, std::optional<int> timeout, std::string roomId)
      {
      // ignore unused param
      (void)(from);(void)(timeout);(void)(roomId);
      
      
              return BaseJob::EmptyBody{};

      };

      

PeekEventsJob::PeekEventsJob(
        std::string serverUrl
        , std::string _accessToken
        ,
        std::string from, std::optional<int> timeout, std::string roomId)
      : BaseJob(std::move(serverUrl),
          std::string("/_matrix/client/r0") + "/events",
          GET,
          _accessToken,
          ReturnType::Json,
            buildBody(from, timeout, roomId)
              , buildQuery(from, timeout, roomId)
                )
        {
        }

          bool PeekEventsJob::success(Response r)
          {
            return BaseJob::success(r)
            
              && isBodyJson(r.body)
          ;
          }


    
    std::string PeekEventsJob::start(Response r)
    {
    if (jsonBody(r).get()
    .contains("start"s)) {
    return
    jsonBody(r).get()["start"s]
    /*.get<std::string>()*/;}
    else { return std::string(  );}
    }

    
    std::string PeekEventsJob::end(Response r)
    {
    if (jsonBody(r).get()
    .contains("end"s)) {
    return
    jsonBody(r).get()["end"s]
    /*.get<std::string>()*/;}
    else { return std::string(  );}
    }

    
    EventList PeekEventsJob::chunk(Response r)
    {
    if (jsonBody(r).get()
    .contains("chunk"s)) {
    return
    jsonBody(r).get()["chunk"s]
    /*.get<EventList>()*/;}
    else { return EventList(  );}
    }

}
