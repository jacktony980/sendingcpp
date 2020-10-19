/******************************************************************************
 * THIS FILE IS GENERATED - ANY EDITS WILL BE OVERWRITTEN
 */

#include <algorithm>

#include "event_context.hpp"

namespace Kazv
{


BaseJob::Query GetEventContextJob::buildQuery(
std::optional<int> limit, std::string filter)
{
BaseJob::Query _q;
  
    addToQueryIfNeeded(_q, "limit"s, limit);
  
    addToQueryIfNeeded(_q, "filter"s, filter);
return _q;
}

    BaseJob::Body GetEventContextJob::buildBody(std::string roomId, std::string eventId, std::optional<int> limit, std::string filter)
      {
      // ignore unused param
      (void)(roomId);(void)(eventId);(void)(limit);(void)(filter);
      
      
              return BaseJob::EmptyBody{};

      };

      

GetEventContextJob::GetEventContextJob(
        std::string serverUrl
        , std::string _accessToken
        ,
        std::string roomId, std::string eventId, std::optional<int> limit, std::string filter)
      : BaseJob(std::move(serverUrl),
          std::string("/_matrix/client/r0") + "/rooms/" + roomId + "/context/" + eventId,
          GET,
          _accessToken,
          ReturnType::Json,
            buildBody(roomId, eventId, limit, filter)
              , buildQuery(limit, filter)
                )
        {
        }

          bool GetEventContextJob::success(Response r)
          {
            return BaseJob::success(r)
            
              && isBodyJson(r.body)
          ;
          }


    
    std::string GetEventContextJob::start(Response r)
    {
    if (jsonBody(r).get()
    .contains("start"s)) {
    return
    jsonBody(r).get()["start"s]
    /*.get<std::string>()*/;}
    else { return std::string(  );}
    }

    
    std::string GetEventContextJob::end(Response r)
    {
    if (jsonBody(r).get()
    .contains("end"s)) {
    return
    jsonBody(r).get()["end"s]
    /*.get<std::string>()*/;}
    else { return std::string(  );}
    }

    
    EventList GetEventContextJob::eventsBefore(Response r)
    {
    if (jsonBody(r).get()
    .contains("events_before"s)) {
    return
    jsonBody(r).get()["events_before"s]
    /*.get<EventList>()*/;}
    else { return EventList(  );}
    }

    
    JsonWrap GetEventContextJob::event(Response r)
    {
    if (jsonBody(r).get()
    .contains("event"s)) {
    return
    jsonBody(r).get()["event"s]
    /*.get<JsonWrap>()*/;}
    else { return JsonWrap(  );}
    }

    
    EventList GetEventContextJob::eventsAfter(Response r)
    {
    if (jsonBody(r).get()
    .contains("events_after"s)) {
    return
    jsonBody(r).get()["events_after"s]
    /*.get<EventList>()*/;}
    else { return EventList(  );}
    }

    
    EventList GetEventContextJob::state(Response r)
    {
    if (jsonBody(r).get()
    .contains("state"s)) {
    return
    jsonBody(r).get()["state"s]
    /*.get<EventList>()*/;}
    else { return EventList(  );}
    }

}
