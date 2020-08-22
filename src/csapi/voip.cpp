/******************************************************************************
 * THIS FILE IS GENERATED - ANY EDITS WILL BE OVERWRITTEN
 */

#include "voip.hpp"

namespace Kazv
{
  

    BaseJob::Body GetTurnServerJob::buildBody()
      {
      // ignore unused param
      
      
      
              return BaseJob::EmptyBody{};

      };

GetTurnServerJob::GetTurnServerJob(
        std::string serverUrl
        , std::string _accessToken
        
        )
      : BaseJob(std::move(serverUrl),
          std::string("/_matrix/client/r0") + "/voip/turnServer",
          GET,
          _accessToken,
          ReturnType::Json,
            buildBody()
      )
        {
        
        
        }


}
