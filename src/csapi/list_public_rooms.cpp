/******************************************************************************
 * THIS FILE IS GENERATED - ANY EDITS WILL BE OVERWRITTEN
 */

#include <algorithm>

#include "list_public_rooms.hpp"

namespace Kazv
{


BaseJob::Query GetRoomVisibilityOnDirectoryJob::buildQuery(
)
{
BaseJob::Query _q;

return _q;
}

    BaseJob::Body GetRoomVisibilityOnDirectoryJob::buildBody(std::string roomId)
      {
      // ignore unused param
      (void)(roomId);
      
      
              return BaseJob::EmptyBody{};

      };

      

GetRoomVisibilityOnDirectoryJob::GetRoomVisibilityOnDirectoryJob(
        std::string serverUrl
        
        ,
        std::string roomId)
      : BaseJob(std::move(serverUrl),
          std::string("/_matrix/client/r0") + "/directory/list/room/" + roomId,
          GET,
           {} ,
          ReturnType::Json,
            buildBody(roomId)
              , buildQuery()
                )
        {
        }

          bool GetRoomVisibilityOnDirectoryJob::success(Response r)
          {
            return BaseJob::success(r)
            
              && isBodyJson(r.body)
          ;
          }


    
    std::string GetRoomVisibilityOnDirectoryJob::visibility(Response r)
    {
    if (jsonBody(r).get()
    .contains("visibility"s)) {
    return
    jsonBody(r).get()["visibility"s]
    /*.get<std::string>()*/;}
    else { return std::string(  );}
    }



BaseJob::Query SetRoomVisibilityOnDirectoryJob::buildQuery(
)
{
BaseJob::Query _q;

return _q;
}

    BaseJob::Body SetRoomVisibilityOnDirectoryJob::buildBody(std::string roomId, std::string visibility)
      {
      // ignore unused param
      (void)(roomId);(void)(visibility);
      
        json _data
        ;
        
          
            addToJsonIfNeeded(_data, "visibility"s, visibility);
        return BaseJob::JsonBody(_data);
        

      };

      

SetRoomVisibilityOnDirectoryJob::SetRoomVisibilityOnDirectoryJob(
        std::string serverUrl
        , std::string _accessToken
        ,
        std::string roomId, std::string visibility)
      : BaseJob(std::move(serverUrl),
          std::string("/_matrix/client/r0") + "/directory/list/room/" + roomId,
          PUT,
          _accessToken,
          ReturnType::Json,
            buildBody(roomId, visibility)
              , buildQuery()
                )
        {
        }

          bool SetRoomVisibilityOnDirectoryJob::success(Response r)
          {
            return BaseJob::success(r)
            
              && isBodyJson(r.body)
          ;
          }




BaseJob::Query GetPublicRoomsJob::buildQuery(
std::optional<int> limit, std::string since, std::string server)
{
BaseJob::Query _q;
  
    addToQueryIfNeeded(_q, "limit"s, limit);
  
    addToQueryIfNeeded(_q, "since"s, since);
  
    addToQueryIfNeeded(_q, "server"s, server);
return _q;
}

    BaseJob::Body GetPublicRoomsJob::buildBody(std::optional<int> limit, std::string since, std::string server)
      {
      // ignore unused param
      (void)(limit);(void)(since);(void)(server);
      
      
              return BaseJob::EmptyBody{};

      };

      

GetPublicRoomsJob::GetPublicRoomsJob(
        std::string serverUrl
        
        ,
        std::optional<int> limit, std::string since, std::string server)
      : BaseJob(std::move(serverUrl),
          std::string("/_matrix/client/r0") + "/publicRooms",
          GET,
           {} ,
          ReturnType::Json,
            buildBody(limit, since, server)
              , buildQuery(limit, since, server)
                )
        {
        }

          bool GetPublicRoomsJob::success(Response r)
          {
            return BaseJob::success(r)
            
              && isBodyJson(r.body)
            && jsonBody(r).get().contains("chunk"s)
          ;
          }


    
    immer::array<PublicRoomsChunk> GetPublicRoomsJob::chunk(Response r)
    {
    if (jsonBody(r).get()
    .contains("chunk"s)) {
    return
    jsonBody(r).get()["chunk"s]
    /*.get<immer::array<PublicRoomsChunk>>()*/;}
    else { return immer::array<PublicRoomsChunk>(  );}
    }

    
    std::string GetPublicRoomsJob::nextBatch(Response r)
    {
    if (jsonBody(r).get()
    .contains("next_batch"s)) {
    return
    jsonBody(r).get()["next_batch"s]
    /*.get<std::string>()*/;}
    else { return std::string(  );}
    }

    
    std::string GetPublicRoomsJob::prevBatch(Response r)
    {
    if (jsonBody(r).get()
    .contains("prev_batch"s)) {
    return
    jsonBody(r).get()["prev_batch"s]
    /*.get<std::string>()*/;}
    else { return std::string(  );}
    }

    
    std::optional<int> GetPublicRoomsJob::totalRoomCountEstimate(Response r)
    {
    if (jsonBody(r).get()
    .contains("total_room_count_estimate"s)) {
    return
    jsonBody(r).get()["total_room_count_estimate"s]
    /*.get<int>()*/;}
    else { return std::optional<int>(  );}
    }



BaseJob::Query QueryPublicRoomsJob::buildQuery(
std::string server)
{
BaseJob::Query _q;
  
    addToQueryIfNeeded(_q, "server"s, server);
return _q;
}

    BaseJob::Body QueryPublicRoomsJob::buildBody(std::string server, std::optional<int> limit, std::string since, std::optional<Filter> filter, std::optional<bool> includeAllNetworks, std::string thirdPartyInstanceId)
      {
      // ignore unused param
      (void)(server);(void)(limit);(void)(since);(void)(filter);(void)(includeAllNetworks);(void)(thirdPartyInstanceId);
      
        json _data
        ;
        
          
            addToJsonIfNeeded(_data, "limit"s, limit);
          
            addToJsonIfNeeded(_data, "since"s, since);
          
            addToJsonIfNeeded(_data, "filter"s, filter);
          
            addToJsonIfNeeded(_data, "include_all_networks"s, includeAllNetworks);
          
            addToJsonIfNeeded(_data, "third_party_instance_id"s, thirdPartyInstanceId);
        return BaseJob::JsonBody(_data);
        

      };

      

QueryPublicRoomsJob::QueryPublicRoomsJob(
        std::string serverUrl
        , std::string _accessToken
        ,
        std::string server, std::optional<int> limit, std::string since, std::optional<Filter> filter, std::optional<bool> includeAllNetworks, std::string thirdPartyInstanceId)
      : BaseJob(std::move(serverUrl),
          std::string("/_matrix/client/r0") + "/publicRooms",
          POST,
          _accessToken,
          ReturnType::Json,
            buildBody(server, limit, since, filter, includeAllNetworks, thirdPartyInstanceId)
              , buildQuery(server)
                )
        {
        }

          bool QueryPublicRoomsJob::success(Response r)
          {
            return BaseJob::success(r)
            
              && isBodyJson(r.body)
            && jsonBody(r).get().contains("chunk"s)
          ;
          }


    
    immer::array<PublicRoomsChunk> QueryPublicRoomsJob::chunk(Response r)
    {
    if (jsonBody(r).get()
    .contains("chunk"s)) {
    return
    jsonBody(r).get()["chunk"s]
    /*.get<immer::array<PublicRoomsChunk>>()*/;}
    else { return immer::array<PublicRoomsChunk>(  );}
    }

    
    std::string QueryPublicRoomsJob::nextBatch(Response r)
    {
    if (jsonBody(r).get()
    .contains("next_batch"s)) {
    return
    jsonBody(r).get()["next_batch"s]
    /*.get<std::string>()*/;}
    else { return std::string(  );}
    }

    
    std::string QueryPublicRoomsJob::prevBatch(Response r)
    {
    if (jsonBody(r).get()
    .contains("prev_batch"s)) {
    return
    jsonBody(r).get()["prev_batch"s]
    /*.get<std::string>()*/;}
    else { return std::string(  );}
    }

    
    std::optional<int> QueryPublicRoomsJob::totalRoomCountEstimate(Response r)
    {
    if (jsonBody(r).get()
    .contains("total_room_count_estimate"s)) {
    return
    jsonBody(r).get()["total_room_count_estimate"s]
    /*.get<int>()*/;}
    else { return std::optional<int>(  );}
    }

}
