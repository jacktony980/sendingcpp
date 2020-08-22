/******************************************************************************
 * THIS FILE IS GENERATED - ANY EDITS WILL BE OVERWRITTEN
 */

#include "logout.hpp"

namespace Kazv
{
  

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
      )
        {
        
        
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
      )
        {
        
        
        }


}
