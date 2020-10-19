/******************************************************************************
 * THIS FILE IS GENERATED - ANY EDITS WILL BE OVERWRITTEN
 */

#include <algorithm>

#include "openid.hpp"

namespace Kazv
{


BaseJob::Query RequestOpenIdTokenJob::buildQuery(
)
{
BaseJob::Query _q;

return _q;
}

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
              , buildQuery()
                )
        {
        }

          bool RequestOpenIdTokenJob::success(Response r)
          {
            return BaseJob::success(r)
            
              && isBodyJson(r.body)
          ;
          }


}
