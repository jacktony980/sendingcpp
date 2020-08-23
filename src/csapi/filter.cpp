/******************************************************************************
 * THIS FILE IS GENERATED - ANY EDITS WILL BE OVERWRITTEN
 */

#include <algorithm>

#include "filter.hpp"

namespace Kazv
{


BaseJob::Query DefineFilterJob::buildQuery(
)
{
BaseJob::Query _q;

return _q;
}

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
              , buildQuery()
                )
        {
        }

          bool DefineFilterJob::success(Response r)
          {
            return BaseJob::success(r)
            
              && isBodyJson(r.body)
            && jsonBody(r).get().contains("filter_id"s)
          ;
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



BaseJob::Query GetFilterJob::buildQuery(
)
{
BaseJob::Query _q;

return _q;
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
              , buildQuery()
                )
        {
        }

          bool GetFilterJob::success(Response r)
          {
            return BaseJob::success(r)
            
              && isBodyJson(r.body)
          ;
          }


}
