/******************************************************************************
 * THIS FILE IS GENERATED - ANY EDITS WILL BE OVERWRITTEN
 */

#include "list_joined_rooms.hpp"

namespace Kazv
{
  

    BaseJob::Body GetJoinedRoomsJob::buildBody()
      {
      // ignore unused param
      
      
      
              return BaseJob::EmptyBody{};

      };

GetJoinedRoomsJob::GetJoinedRoomsJob(
        std::string serverUrl
        , std::string _accessToken
        
        )
      : BaseJob(std::move(serverUrl),
          std::string("/_matrix/client/r0") + "/joined_rooms",
          GET,
          _accessToken,
          ReturnType::Json,
            buildBody()
      )
        {
        
        
          //addExpectedKey("joined_rooms");
        }


    
    immer::array<std::string> GetJoinedRoomsJob::joinedRooms(Response r)
    {
    if (jsonBody(r).get()
    .contains("joined_rooms"s)) {
    return
    jsonBody(r).get()["joined_rooms"s]
    /*.get<immer::array<std::string>>()*/;}
    else { return immer::array<std::string>(  );}
    }

}
