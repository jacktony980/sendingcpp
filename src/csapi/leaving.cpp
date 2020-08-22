/******************************************************************************
 * THIS FILE IS GENERATED - ANY EDITS WILL BE OVERWRITTEN
 */

#include "leaving.hpp"

namespace Kazv
{
  

    BaseJob::Body LeaveRoomJob::buildBody(std::string roomId)
      {
      // ignore unused param
      (void)(roomId);
      
      
              return BaseJob::EmptyBody{};

      };

LeaveRoomJob::LeaveRoomJob(
        std::string serverUrl
        , std::string _accessToken
        ,
        std::string roomId)
      : BaseJob(std::move(serverUrl),
          std::string("/_matrix/client/r0") + "/rooms/" + roomId + "/leave",
          POST,
          _accessToken,
          ReturnType::Json,
            buildBody(roomId)
      )
        {
        
        
        }


  

    BaseJob::Body ForgetRoomJob::buildBody(std::string roomId)
      {
      // ignore unused param
      (void)(roomId);
      
      
              return BaseJob::EmptyBody{};

      };

ForgetRoomJob::ForgetRoomJob(
        std::string serverUrl
        , std::string _accessToken
        ,
        std::string roomId)
      : BaseJob(std::move(serverUrl),
          std::string("/_matrix/client/r0") + "/rooms/" + roomId + "/forget",
          POST,
          _accessToken,
          ReturnType::Json,
            buildBody(roomId)
      )
        {
        
        
        }


}
