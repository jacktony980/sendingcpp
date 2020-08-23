/******************************************************************************
 * THIS FILE IS GENERATED - ANY EDITS WILL BE OVERWRITTEN
 */

#include <algorithm>

#include "voip.hpp"

namespace Kazv
{


BaseJob::Query GetTurnServerJob::buildQuery(
)
{
BaseJob::Query _q;

return _q;
}

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
              , buildQuery()
                )
        {
        }

          bool GetTurnServerJob::success(Response r)
          {
            return BaseJob::success(r)
            
              && isBodyJson(r.body)
          ;
          }


}
