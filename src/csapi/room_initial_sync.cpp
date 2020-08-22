/******************************************************************************
 * THIS FILE IS GENERATED - ANY EDITS WILL BE OVERWRITTEN
 */

#include "room_initial_sync.hpp"

namespace Kazv
{
  

    BaseJob::Body RoomInitialSyncJob::buildBody(std::string roomId)
      {
      // ignore unused param
      (void)(roomId);
      
      
              return BaseJob::EmptyBody{};

      };

RoomInitialSyncJob::RoomInitialSyncJob(
        std::string serverUrl
        , std::string _accessToken
        ,
        std::string roomId)
      : BaseJob(std::move(serverUrl),
          std::string("/_matrix/client/r0") + "/rooms/" + roomId + "/initialSync",
          GET,
          _accessToken,
          ReturnType::Json,
            buildBody(roomId)
      )
        {
        
        
          //addExpectedKey("room_id");
        }


    
    std::string RoomInitialSyncJob::roomId(Response r)
    {
    if (jsonBody(r).get()
    .contains("room_id"s)) {
    return
    jsonBody(r).get()["room_id"s]
    /*.get<std::string>()*/;}
    else { return std::string(  );}
    }

    
    std::string RoomInitialSyncJob::membership(Response r)
    {
    if (jsonBody(r).get()
    .contains("membership"s)) {
    return
    jsonBody(r).get()["membership"s]
    /*.get<std::string>()*/;}
    else { return std::string(  );}
    }

    
    std::optional<RoomInitialSyncJob::PaginationChunk> RoomInitialSyncJob::messages(Response r)
    {
    if (jsonBody(r).get()
    .contains("messages"s)) {
    return
    jsonBody(r).get()["messages"s]
    /*.get<PaginationChunk>()*/;}
    else { return std::optional<PaginationChunk>(  );}
    }

    
    EventList RoomInitialSyncJob::state(Response r)
    {
    if (jsonBody(r).get()
    .contains("state"s)) {
    return
    jsonBody(r).get()["state"s]
    /*.get<EventList>()*/;}
    else { return EventList(  );}
    }

    
    std::string RoomInitialSyncJob::visibility(Response r)
    {
    if (jsonBody(r).get()
    .contains("visibility"s)) {
    return
    jsonBody(r).get()["visibility"s]
    /*.get<std::string>()*/;}
    else { return std::string(  );}
    }

    
    EventList RoomInitialSyncJob::accountData(Response r)
    {
    if (jsonBody(r).get()
    .contains("account_data"s)) {
    return
    jsonBody(r).get()["account_data"s]
    /*.get<EventList>()*/;}
    else { return EventList(  );}
    }

}
