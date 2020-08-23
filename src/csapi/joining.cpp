/******************************************************************************
 * THIS FILE IS GENERATED - ANY EDITS WILL BE OVERWRITTEN
 */

#include <algorithm>

#include "joining.hpp"

namespace Kazv
{


BaseJob::Query JoinRoomByIdJob::buildQuery(
)
{
BaseJob::Query _q;

return _q;
}

    BaseJob::Body JoinRoomByIdJob::buildBody(std::string roomId, std::optional<ThirdPartySigned> thirdPartySigned)
      {
      // ignore unused param
      (void)(roomId);(void)(thirdPartySigned);
      
        json _data
        ;
        
          
            addToJsonIfNeeded(_data, "third_party_signed"s, thirdPartySigned);
        return BaseJob::JsonBody(_data);
        

      };

      

JoinRoomByIdJob::JoinRoomByIdJob(
        std::string serverUrl
        , std::string _accessToken
        ,
        std::string roomId, std::optional<ThirdPartySigned> thirdPartySigned)
      : BaseJob(std::move(serverUrl),
          std::string("/_matrix/client/r0") + "/rooms/" + roomId + "/join",
          POST,
          _accessToken,
          ReturnType::Json,
            buildBody(roomId, thirdPartySigned)
              , buildQuery()
                )
        {
        }

          bool JoinRoomByIdJob::success(Response r)
          {
            return BaseJob::success(r)
            
              && isBodyJson(r.body)
            && jsonBody(r).get().contains("room_id"s)
          ;
          }


    
    std::string JoinRoomByIdJob::roomId(Response r)
    {
    if (jsonBody(r).get()
    .contains("room_id"s)) {
    return
    jsonBody(r).get()["room_id"s]
    /*.get<std::string>()*/;}
    else { return std::string(  );}
    }



BaseJob::Query JoinRoomJob::buildQuery(
immer::array<std::string> serverName)
{
BaseJob::Query _q;
  
    addToQueryIfNeeded(_q, "server_name"s, serverName);
return _q;
}

    BaseJob::Body JoinRoomJob::buildBody(std::string roomIdOrAlias, immer::array<std::string> serverName, std::optional<ThirdPartySigned> thirdPartySigned)
      {
      // ignore unused param
      (void)(roomIdOrAlias);(void)(serverName);(void)(thirdPartySigned);
      
        json _data
        ;
        
          
            addToJsonIfNeeded(_data, "third_party_signed"s, thirdPartySigned);
        return BaseJob::JsonBody(_data);
        

      };

      

JoinRoomJob::JoinRoomJob(
        std::string serverUrl
        , std::string _accessToken
        ,
        std::string roomIdOrAlias, immer::array<std::string> serverName, std::optional<ThirdPartySigned> thirdPartySigned)
      : BaseJob(std::move(serverUrl),
          std::string("/_matrix/client/r0") + "/join/" + roomIdOrAlias,
          POST,
          _accessToken,
          ReturnType::Json,
            buildBody(roomIdOrAlias, serverName, thirdPartySigned)
              , buildQuery(serverName)
                )
        {
        }

          bool JoinRoomJob::success(Response r)
          {
            return BaseJob::success(r)
            
              && isBodyJson(r.body)
            && jsonBody(r).get().contains("room_id"s)
          ;
          }


    
    std::string JoinRoomJob::roomId(Response r)
    {
    if (jsonBody(r).get()
    .contains("room_id"s)) {
    return
    jsonBody(r).get()["room_id"s]
    /*.get<std::string>()*/;}
    else { return std::string(  );}
    }

}
