/******************************************************************************
 * THIS FILE IS GENERATED - ANY EDITS WILL BE OVERWRITTEN
 */

#include "directory.hpp"

namespace Kazv
{
  

    BaseJob::Body SetRoomAliasJob::buildBody(std::string roomAlias, std::string roomId)
      {
      // ignore unused param
      (void)(roomAlias);(void)(roomId);
      
        json _data
        ;
        
            _data["room_id"s] = roomId;
          
        return BaseJob::JsonBody(_data);
        

      };

SetRoomAliasJob::SetRoomAliasJob(
        std::string serverUrl
        , std::string _accessToken
        ,
        std::string roomAlias, std::string roomId)
      : BaseJob(std::move(serverUrl),
          std::string("/_matrix/client/r0") + "/directory/room/" + roomAlias,
          PUT,
          _accessToken,
          ReturnType::Json,
            buildBody(roomAlias, roomId)
      )
        {
        
        
        }


  

    BaseJob::Body GetRoomIdByAliasJob::buildBody(std::string roomAlias)
      {
      // ignore unused param
      (void)(roomAlias);
      
      
              return BaseJob::EmptyBody{};

      };

GetRoomIdByAliasJob::GetRoomIdByAliasJob(
        std::string serverUrl
        
        ,
        std::string roomAlias)
      : BaseJob(std::move(serverUrl),
          std::string("/_matrix/client/r0") + "/directory/room/" + roomAlias,
          GET,
           {} ,
          ReturnType::Json,
            buildBody(roomAlias)
      )
        {
        
        
        }


    
    std::string GetRoomIdByAliasJob::roomId(Response r)
    {
    if (jsonBody(r).get()
    .contains("room_id"s)) {
    return
    jsonBody(r).get()["room_id"s]
    /*.get<std::string>()*/;}
    else { return std::string(  );}
    }

    
    immer::array<std::string> GetRoomIdByAliasJob::servers(Response r)
    {
    if (jsonBody(r).get()
    .contains("servers"s)) {
    return
    jsonBody(r).get()["servers"s]
    /*.get<immer::array<std::string>>()*/;}
    else { return immer::array<std::string>(  );}
    }

  

    BaseJob::Body DeleteRoomAliasJob::buildBody(std::string roomAlias)
      {
      // ignore unused param
      (void)(roomAlias);
      
      
              return BaseJob::EmptyBody{};

      };

DeleteRoomAliasJob::DeleteRoomAliasJob(
        std::string serverUrl
        , std::string _accessToken
        ,
        std::string roomAlias)
      : BaseJob(std::move(serverUrl),
          std::string("/_matrix/client/r0") + "/directory/room/" + roomAlias,
          DELETE,
          _accessToken,
          ReturnType::Json,
            buildBody(roomAlias)
      )
        {
        
        
        }


  

    BaseJob::Body GetLocalAliasesJob::buildBody(std::string roomId)
      {
      // ignore unused param
      (void)(roomId);
      
      
              return BaseJob::EmptyBody{};

      };

GetLocalAliasesJob::GetLocalAliasesJob(
        std::string serverUrl
        , std::string _accessToken
        ,
        std::string roomId)
      : BaseJob(std::move(serverUrl),
          std::string("/_matrix/client/r0") + "/rooms/" + roomId + "/aliases",
          GET,
          _accessToken,
          ReturnType::Json,
            buildBody(roomId)
      )
        {
        
        
          //addExpectedKey("aliases");
        }


    
    immer::array<std::string> GetLocalAliasesJob::aliases(Response r)
    {
    if (jsonBody(r).get()
    .contains("aliases"s)) {
    return
    jsonBody(r).get()["aliases"s]
    /*.get<immer::array<std::string>>()*/;}
    else { return immer::array<std::string>(  );}
    }

}
