/******************************************************************************
 * THIS FILE IS GENERATED - ANY EDITS WILL BE OVERWRITTEN
 */

#include "appservice_room_directory.hpp"

namespace Kazv
{
  

    BaseJob::Body UpdateAppserviceRoomDirectoryVsibilityJob::buildBody(std::string networkId, std::string roomId, std::string visibility)
      {
      // ignore unused param
      (void)(networkId);(void)(roomId);(void)(visibility);
      
        json _data
        ;
        
            _data["visibility"s] = visibility;
          
        return BaseJob::JsonBody(_data);
        

      };

UpdateAppserviceRoomDirectoryVsibilityJob::UpdateAppserviceRoomDirectoryVsibilityJob(
        std::string serverUrl
        , std::string _accessToken
        ,
        std::string networkId, std::string roomId, std::string visibility)
      : BaseJob(std::move(serverUrl),
          std::string("/_matrix/client/r0") + "/directory/list/appservice/" + networkId + "/" + roomId,
          PUT,
          _accessToken,
          ReturnType::Json,
            buildBody(networkId, roomId, visibility)
      )
        {
        
        
        }


}
