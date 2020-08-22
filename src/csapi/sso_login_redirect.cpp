/******************************************************************************
 * THIS FILE IS GENERATED - ANY EDITS WILL BE OVERWRITTEN
 */

#include "sso_login_redirect.hpp"

namespace Kazv
{

BaseJob::Query RedirectToSSOJob::buildQuery(
std::string redirectUrl)
{
BaseJob::Query _q;
    addToQuery(_q, "redirectUrl"s, redirectUrl);
  
return _q;
}

    BaseJob::Body RedirectToSSOJob::buildBody(std::string redirectUrl)
      {
      // ignore unused param
      (void)(redirectUrl);
      
      
              return BaseJob::EmptyBody{};

      };

RedirectToSSOJob::RedirectToSSOJob(
        std::string serverUrl
        
        ,
        std::string redirectUrl)
      : BaseJob(std::move(serverUrl),
          std::string("/_matrix/client/r0") + "/login/sso/redirect",
          GET,
           {} ,
          ReturnType::Json,
            buildBody(redirectUrl)
      , buildQuery(redirectUrl))
        {
        
        
        }

        

}
