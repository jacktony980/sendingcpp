/******************************************************************************
 * THIS FILE IS GENERATED - ANY EDITS WILL BE OVERWRITTEN
 */

#include <algorithm>

#include "list_joined_rooms.hpp"

namespace Kazv
{


BaseJob::Query GetJoinedRoomsJob::buildQuery(
)
{
BaseJob::Query _q;

return _q;
}

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
              , buildQuery()
                )
        {
        }

          bool GetJoinedRoomsJob::success(Response r)
          {
            return BaseJob::success(r)
            
              && isBodyJson(r.body)
            && jsonBody(r).get().contains("joined_rooms"s)
          ;
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
