/******************************************************************************
 * THIS FILE IS GENERATED - ANY EDITS WILL BE OVERWRITTEN
 */

#include <algorithm>

#include "versions.hpp"

namespace Kazv
{


BaseJob::Query GetVersionsJob::buildQuery(
)
{
BaseJob::Query _q;

return _q;
}

    BaseJob::Body GetVersionsJob::buildBody()
      {
      // ignore unused param
      
      
      
              return BaseJob::EmptyBody{};

      };

      

GetVersionsJob::GetVersionsJob(
        std::string serverUrl
        
        
        )
      : BaseJob(std::move(serverUrl),
          std::string("/_matrix/client") + "/versions",
          GET,
           {} ,
          ReturnType::Json,
            buildBody()
              , buildQuery()
                )
        {
        }

          bool GetVersionsJob::success(Response r)
          {
            return BaseJob::success(r)
            
              && isBodyJson(r.body)
            && jsonBody(r).get().contains("versions"s)
          ;
          }


    
    immer::array<std::string> GetVersionsJob::versions(Response r)
    {
    if (jsonBody(r).get()
    .contains("versions"s)) {
    return
    jsonBody(r).get()["versions"s]
    /*.get<immer::array<std::string>>()*/;}
    else { return immer::array<std::string>(  );}
    }

    
    immer::map<std::string, bool> GetVersionsJob::unstableFeatures(Response r)
    {
    if (jsonBody(r).get()
    .contains("unstable_features"s)) {
    return
    jsonBody(r).get()["unstable_features"s]
    /*.get<immer::map<std::string, bool>>()*/;}
    else { return immer::map<std::string, bool>(  );}
    }

}
