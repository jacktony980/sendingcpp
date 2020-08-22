/******************************************************************************
 * THIS FILE IS GENERATED - ANY EDITS WILL BE OVERWRITTEN
 */

#include "openid.hpp"

namespace Kazv
{
  

    BaseJob::Body RequestOpenIdTokenJob::buildBody(std::string userId, JsonWrap body)
      {
      // ignore unused param
      (void)(userId);(void)(body);
        return 
          BaseJob::JsonBody(body);
      
          

      };

RequestOpenIdTokenJob::RequestOpenIdTokenJob(
        std::string serverUrl
        , std::string _accessToken
        ,
        std::string userId, JsonWrap body)
      : BaseJob(std::move(serverUrl),
          std::string("/_matrix/client/r0") + "/user/" + userId + "/openid/request_token",
          POST,
          _accessToken,
          ReturnType::Json,
            buildBody(userId, body)
      )
        {
        
        
        }


}
