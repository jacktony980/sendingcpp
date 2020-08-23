/******************************************************************************
 * THIS FILE IS GENERATED - ANY EDITS WILL BE OVERWRITTEN
 */

#include <algorithm>

#include "notifications.hpp"

namespace Kazv
{


BaseJob::Query GetNotificationsJob::buildQuery(
std::string from, std::optional<int> limit, std::string only)
{
BaseJob::Query _q;
  
    addToQueryIfNeeded(_q, "from"s, from);
  
    addToQueryIfNeeded(_q, "limit"s, limit);
  
    addToQueryIfNeeded(_q, "only"s, only);
return _q;
}

    BaseJob::Body GetNotificationsJob::buildBody(std::string from, std::optional<int> limit, std::string only)
      {
      // ignore unused param
      (void)(from);(void)(limit);(void)(only);
      
      
              return BaseJob::EmptyBody{};

      };

      

GetNotificationsJob::GetNotificationsJob(
        std::string serverUrl
        , std::string _accessToken
        ,
        std::string from, std::optional<int> limit, std::string only)
      : BaseJob(std::move(serverUrl),
          std::string("/_matrix/client/r0") + "/notifications",
          GET,
          _accessToken,
          ReturnType::Json,
            buildBody(from, limit, only)
              , buildQuery(from, limit, only)
                )
        {
        }

          bool GetNotificationsJob::success(Response r)
          {
            return BaseJob::success(r)
            
              && isBodyJson(r.body)
            && jsonBody(r).get().contains("notifications"s)
          ;
          }


    
    std::string GetNotificationsJob::nextToken(Response r)
    {
    if (jsonBody(r).get()
    .contains("next_token"s)) {
    return
    jsonBody(r).get()["next_token"s]
    /*.get<std::string>()*/;}
    else { return std::string(  );}
    }

    
    immer::array<GetNotificationsJob::Notification> GetNotificationsJob::notifications(Response r)
    {
    if (jsonBody(r).get()
    .contains("notifications"s)) {
    return
    jsonBody(r).get()["notifications"s]
    /*.get<immer::array<Notification>>()*/;}
    else { return immer::array<Notification>(  );}
    }

}
