/******************************************************************************
 * THIS FILE IS GENERATED - ANY EDITS WILL BE OVERWRITTEN
 */

#include "versions.hpp"

namespace Kazv
{
  

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
      )
        {
        
        
          //addExpectedKey("versions");
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
