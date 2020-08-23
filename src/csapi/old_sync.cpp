/******************************************************************************
 * THIS FILE IS GENERATED - ANY EDITS WILL BE OVERWRITTEN
 */

#include <algorithm>

#include "old_sync.hpp"

namespace Kazv
{


BaseJob::Query GetEventsJob::buildQuery(
std::string from, std::optional<int> timeout)
{
BaseJob::Query _q;
  
    addToQueryIfNeeded(_q, "from"s, from);
  
    addToQueryIfNeeded(_q, "timeout"s, timeout);
return _q;
}

    BaseJob::Body GetEventsJob::buildBody(std::string from, std::optional<int> timeout)
      {
      // ignore unused param
      (void)(from);(void)(timeout);
      
      
              return BaseJob::EmptyBody{};

      };

      

GetEventsJob::GetEventsJob(
        std::string serverUrl
        , std::string _accessToken
        ,
        std::string from, std::optional<int> timeout)
      : BaseJob(std::move(serverUrl),
          std::string("/_matrix/client/r0") + "/events",
          GET,
          _accessToken,
          ReturnType::Json,
            buildBody(from, timeout)
              , buildQuery(from, timeout)
                )
        {
        }

          bool GetEventsJob::success(Response r)
          {
            return BaseJob::success(r)
            
              && isBodyJson(r.body)
          ;
          }


    
    std::string GetEventsJob::start(Response r)
    {
    if (jsonBody(r).get()
    .contains("start"s)) {
    return
    jsonBody(r).get()["start"s]
    /*.get<std::string>()*/;}
    else { return std::string(  );}
    }

    
    std::string GetEventsJob::end(Response r)
    {
    if (jsonBody(r).get()
    .contains("end"s)) {
    return
    jsonBody(r).get()["end"s]
    /*.get<std::string>()*/;}
    else { return std::string(  );}
    }

    
    EventList GetEventsJob::chunk(Response r)
    {
    if (jsonBody(r).get()
    .contains("chunk"s)) {
    return
    jsonBody(r).get()["chunk"s]
    /*.get<EventList>()*/;}
    else { return EventList(  );}
    }



BaseJob::Query InitialSyncJob::buildQuery(
std::optional<int> limit, std::optional<bool> archived)
{
BaseJob::Query _q;
  
    addToQueryIfNeeded(_q, "limit"s, limit);
  
    addToQueryIfNeeded(_q, "archived"s, archived);
return _q;
}

    BaseJob::Body InitialSyncJob::buildBody(std::optional<int> limit, std::optional<bool> archived)
      {
      // ignore unused param
      (void)(limit);(void)(archived);
      
      
              return BaseJob::EmptyBody{};

      };

      

InitialSyncJob::InitialSyncJob(
        std::string serverUrl
        , std::string _accessToken
        ,
        std::optional<int> limit, std::optional<bool> archived)
      : BaseJob(std::move(serverUrl),
          std::string("/_matrix/client/r0") + "/initialSync",
          GET,
          _accessToken,
          ReturnType::Json,
            buildBody(limit, archived)
              , buildQuery(limit, archived)
                )
        {
        }

          bool InitialSyncJob::success(Response r)
          {
            return BaseJob::success(r)
            
              && isBodyJson(r.body)
            && jsonBody(r).get().contains("end"s)
            && jsonBody(r).get().contains("presence"s)
            && jsonBody(r).get().contains("rooms"s)
          ;
          }


    
    std::string InitialSyncJob::end(Response r)
    {
    if (jsonBody(r).get()
    .contains("end"s)) {
    return
    jsonBody(r).get()["end"s]
    /*.get<std::string>()*/;}
    else { return std::string(  );}
    }

    
    EventList InitialSyncJob::presence(Response r)
    {
    if (jsonBody(r).get()
    .contains("presence"s)) {
    return
    jsonBody(r).get()["presence"s]
    /*.get<EventList>()*/;}
    else { return EventList(  );}
    }

    
    immer::array<InitialSyncJob::RoomInfo> InitialSyncJob::rooms(Response r)
    {
    if (jsonBody(r).get()
    .contains("rooms"s)) {
    return
    jsonBody(r).get()["rooms"s]
    /*.get<immer::array<RoomInfo>>()*/;}
    else { return immer::array<RoomInfo>(  );}
    }

    
    EventList InitialSyncJob::accountData(Response r)
    {
    if (jsonBody(r).get()
    .contains("account_data"s)) {
    return
    jsonBody(r).get()["account_data"s]
    /*.get<EventList>()*/;}
    else { return EventList(  );}
    }



BaseJob::Query GetOneEventJob::buildQuery(
)
{
BaseJob::Query _q;

return _q;
}

    BaseJob::Body GetOneEventJob::buildBody(std::string eventId)
      {
      // ignore unused param
      (void)(eventId);
      
      
              return BaseJob::EmptyBody{};

      };

      

GetOneEventJob::GetOneEventJob(
        std::string serverUrl
        , std::string _accessToken
        ,
        std::string eventId)
      : BaseJob(std::move(serverUrl),
          std::string("/_matrix/client/r0") + "/events/" + eventId,
          GET,
          _accessToken,
          ReturnType::Json,
            buildBody(eventId)
              , buildQuery()
                )
        {
        }

          bool GetOneEventJob::success(Response r)
          {
            return BaseJob::success(r)
            
              && isBodyJson(r.body)
          ;
          }


}
