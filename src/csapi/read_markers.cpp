/******************************************************************************
 * THIS FILE IS GENERATED - ANY EDITS WILL BE OVERWRITTEN
 */

#include "read_markers.hpp"

namespace Kazv
{
  

    BaseJob::Body SetReadMarkerJob::buildBody(std::string roomId, std::string mFullyRead, std::string mRead)
      {
      // ignore unused param
      (void)(roomId);(void)(mFullyRead);(void)(mRead);
      
        json _data
        ;
        
            _data["m.fully_read"s] = mFullyRead;
          
          
            addToJsonIfNeeded(_data, "m.read"s, mRead);
        return BaseJob::JsonBody(_data);
        

      };

SetReadMarkerJob::SetReadMarkerJob(
        std::string serverUrl
        , std::string _accessToken
        ,
        std::string roomId, std::string mFullyRead, std::string mRead)
      : BaseJob(std::move(serverUrl),
          std::string("/_matrix/client/r0") + "/rooms/" + roomId + "/read_markers",
          POST,
          _accessToken,
          ReturnType::Json,
            buildBody(roomId, mFullyRead, mRead)
      )
        {
        
        
        }


}
