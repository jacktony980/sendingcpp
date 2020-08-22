/******************************************************************************
 * THIS FILE IS GENERATED - ANY EDITS WILL BE OVERWRITTEN
 */

#include "filter.hpp"

namespace Kazv
{
  

    BaseJob::Body DefineFilterJob::buildBody(std::string userId, Filter filter)
      {
      // ignore unused param
      (void)(userId);(void)(filter);
        return 
          BaseJob::JsonBody(filter);
      
          

      };

DefineFilterJob::DefineFilterJob(
        std::string serverUrl
        , std::string _accessToken
        ,
        std::string userId, Filter filter)
      : BaseJob(std::move(serverUrl),
          std::string("/_matrix/client/r0") + "/user/" + userId + "/filter",
          POST,
          _accessToken,
          ReturnType::Json,
            buildBody(userId, filter)
      )
        {
        
        
          //addExpectedKey("filter_id");
        }


    
    std::string DefineFilterJob::filterId(Response r)
    {
    if (jsonBody(r).get()
    .contains("filter_id"s)) {
    return
    jsonBody(r).get()["filter_id"s]
    /*.get<std::string>()*/;}
    else { return std::string(  );}
    }

  

    BaseJob::Body GetFilterJob::buildBody(std::string userId, std::string filterId)
      {
      // ignore unused param
      (void)(userId);(void)(filterId);
      
      
              return BaseJob::EmptyBody{};

      };

GetFilterJob::GetFilterJob(
        std::string serverUrl
        , std::string _accessToken
        ,
        std::string userId, std::string filterId)
      : BaseJob(std::move(serverUrl),
          std::string("/_matrix/client/r0") + "/user/" + userId + "/filter/" + filterId,
          GET,
          _accessToken,
          ReturnType::Json,
            buildBody(userId, filterId)
      )
        {
        
        
        }


}
