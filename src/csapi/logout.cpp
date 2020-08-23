/******************************************************************************
 * THIS FILE IS GENERATED - ANY EDITS WILL BE OVERWRITTEN
 */

#include <algorithm>

#include "logout.hpp"

namespace Kazv
{


BaseJob::Query LogoutJob::buildQuery(
)
{
BaseJob::Query _q;

return _q;
}

    BaseJob::Body LogoutJob::buildBody()
      {
      // ignore unused param
      
      
      
              return BaseJob::EmptyBody{};

      };

      

LogoutJob::LogoutJob(
        std::string serverUrl
        , std::string _accessToken
        
        )
      : BaseJob(std::move(serverUrl),
          std::string("/_matrix/client/r0") + "/logout",
          POST,
          _accessToken,
          ReturnType::Json,
            buildBody()
              , buildQuery()
                )
        {
        }

          bool LogoutJob::success(Response r)
          {
            return BaseJob::success(r)
            
              && isBodyJson(r.body)
          ;
          }




BaseJob::Query LogoutAllJob::buildQuery(
)
{
BaseJob::Query _q;

return _q;
}

    BaseJob::Body LogoutAllJob::buildBody()
      {
      // ignore unused param
      
      
      
              return BaseJob::EmptyBody{};

      };

      

LogoutAllJob::LogoutAllJob(
        std::string serverUrl
        , std::string _accessToken
        
        )
      : BaseJob(std::move(serverUrl),
          std::string("/_matrix/client/r0") + "/logout/all",
          POST,
          _accessToken,
          ReturnType::Json,
            buildBody()
              , buildQuery()
                )
        {
        }

          bool LogoutAllJob::success(Response r)
          {
            return BaseJob::success(r)
            
              && isBodyJson(r.body)
          ;
          }


}
