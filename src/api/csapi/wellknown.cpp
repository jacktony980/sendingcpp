/******************************************************************************
 * THIS FILE IS GENERATED - ANY EDITS WILL BE OVERWRITTEN
 */

#include <algorithm>

#include "wellknown.hpp"

namespace Kazv
{


BaseJob::Query GetWellknownJob::buildQuery(
)
{
BaseJob::Query _q;

return _q;
}

    BaseJob::Body GetWellknownJob::buildBody()
      {
      // ignore unused param
      
      
      
              return BaseJob::EmptyBody{};

      };

      

GetWellknownJob::GetWellknownJob(
        std::string serverUrl
        
        
        )
      : BaseJob(std::move(serverUrl),
          std::string("/.well-known") + "/matrix/client",
          GET,
           {} ,
          ReturnType::Json,
            buildBody()
              , buildQuery()
                )
        {
        }

          bool GetWellknownJob::success(Response r)
          {
            return BaseJob::success(r)
            
              && isBodyJson(r.body)
          ;
          }


}
