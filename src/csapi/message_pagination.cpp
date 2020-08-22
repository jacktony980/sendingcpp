/******************************************************************************
 * THIS FILE IS GENERATED - ANY EDITS WILL BE OVERWRITTEN
 */

#include "message_pagination.hpp"

namespace Kazv
{

BaseJob::Query GetRoomEventsJob::buildQuery(
std::string from, std::string to, std::string dir, std::optional<int> limit, std::string filter)
{
BaseJob::Query _q;
    addToQuery(_q, "from"s, from);
  
  
    addToQueryIfNeeded(_q, "to"s, to);
    addToQuery(_q, "dir"s, dir);
  
  
    addToQueryIfNeeded(_q, "limit"s, limit);
  
    addToQueryIfNeeded(_q, "filter"s, filter);
return _q;
}

    BaseJob::Body GetRoomEventsJob::buildBody(std::string roomId, std::string from, std::string dir, std::string to, std::optional<int> limit, std::string filter)
      {
      // ignore unused param
      (void)(roomId);(void)(from);(void)(dir);(void)(to);(void)(limit);(void)(filter);
      
      
              return BaseJob::EmptyBody{};

      };

GetRoomEventsJob::GetRoomEventsJob(
        std::string serverUrl
        , std::string _accessToken
        ,
        std::string roomId, std::string from, std::string dir, std::string to, std::optional<int> limit, std::string filter)
      : BaseJob(std::move(serverUrl),
          std::string("/_matrix/client/r0") + "/rooms/" + roomId + "/messages",
          GET,
          _accessToken,
          ReturnType::Json,
            buildBody(roomId, from, dir, to, limit, filter)
      , buildQuery(from, to, dir, limit, filter))
        {
        
        
        }


    
    std::string GetRoomEventsJob::start(Response r)
    {
    if (jsonBody(r).get()
    .contains("start"s)) {
    return
    jsonBody(r).get()["start"s]
    /*.get<std::string>()*/;}
    else { return std::string(  );}
    }

    
    std::string GetRoomEventsJob::end(Response r)
    {
    if (jsonBody(r).get()
    .contains("end"s)) {
    return
    jsonBody(r).get()["end"s]
    /*.get<std::string>()*/;}
    else { return std::string(  );}
    }

    
    EventList GetRoomEventsJob::chunk(Response r)
    {
    if (jsonBody(r).get()
    .contains("chunk"s)) {
    return
    jsonBody(r).get()["chunk"s]
    /*.get<EventList>()*/;}
    else { return EventList(  );}
    }

    
    EventList GetRoomEventsJob::state(Response r)
    {
    if (jsonBody(r).get()
    .contains("state"s)) {
    return
    jsonBody(r).get()["state"s]
    /*.get<EventList>()*/;}
    else { return EventList(  );}
    }

}
