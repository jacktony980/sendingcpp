/******************************************************************************
 * THIS FILE IS GENERATED - ANY EDITS WILL BE OVERWRITTEN
 */

#include "whoami.hpp"

namespace Kazv
{
  

    BaseJob::Body GetTokenOwnerJob::buildBody()
      {
      // ignore unused param
      
      
      
              return BaseJob::EmptyBody{};

      };

GetTokenOwnerJob::GetTokenOwnerJob(
        std::string serverUrl
        , std::string _accessToken
        
        )
      : BaseJob(std::move(serverUrl),
          std::string("/_matrix/client/r0") + "/account/whoami",
          GET,
          _accessToken,
          ReturnType::Json,
            buildBody()
      )
        {
        
        
          //addExpectedKey("user_id");
        }


    
    std::string GetTokenOwnerJob::userId(Response r)
    {
    if (jsonBody(r).get()
    .contains("user_id"s)) {
    return
    jsonBody(r).get()["user_id"s]
    /*.get<std::string>()*/;}
    else { return std::string(  );}
    }

}
