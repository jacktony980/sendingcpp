/******************************************************************************
 * THIS FILE IS GENERATED - ANY EDITS WILL BE OVERWRITTEN
 */

#include "capabilities.hpp"

namespace Kazv
{
  

    BaseJob::Body GetCapabilitiesJob::buildBody()
      {
      // ignore unused param
      
      
      
              return BaseJob::EmptyBody{};

      };

GetCapabilitiesJob::GetCapabilitiesJob(
        std::string serverUrl
        , std::string _accessToken
        
        )
      : BaseJob(std::move(serverUrl),
          std::string("/_matrix/client/r0") + "/capabilities",
          GET,
          _accessToken,
          ReturnType::Json,
            buildBody()
      )
        {
        
        
          //addExpectedKey("capabilities");
        }


    
    GetCapabilitiesJob::Capabilities GetCapabilitiesJob::capabilities(Response r)
    {
    if (jsonBody(r).get()
    .contains("capabilities"s)) {
    return
    jsonBody(r).get()["capabilities"s]
    /*.get<Capabilities>()*/;}
    else { return Capabilities(  );}
    }

}
