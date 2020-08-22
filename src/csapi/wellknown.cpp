/******************************************************************************
 * THIS FILE IS GENERATED - ANY EDITS WILL BE OVERWRITTEN
 */

#include "wellknown.hpp"

namespace Kazv
{
  

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
      )
        {
        
        
        }


}
