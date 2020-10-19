/******************************************************************************
 * THIS FILE IS GENERATED - ANY EDITS WILL BE OVERWRITTEN
 */

#include <algorithm>

#include "pusher.hpp"

namespace Kazv
{


BaseJob::Query GetPushersJob::buildQuery(
)
{
BaseJob::Query _q;

return _q;
}

    BaseJob::Body GetPushersJob::buildBody()
      {
      // ignore unused param
      
      
      
              return BaseJob::EmptyBody{};

      };

      

GetPushersJob::GetPushersJob(
        std::string serverUrl
        , std::string _accessToken
        
        )
      : BaseJob(std::move(serverUrl),
          std::string("/_matrix/client/r0") + "/pushers",
          GET,
          _accessToken,
          ReturnType::Json,
            buildBody()
              , buildQuery()
                )
        {
        }

          bool GetPushersJob::success(Response r)
          {
            return BaseJob::success(r)
            
              && isBodyJson(r.body)
          ;
          }


    
    immer::array<GetPushersJob::Pusher> GetPushersJob::pushers(Response r)
    {
    if (jsonBody(r).get()
    .contains("pushers"s)) {
    return
    jsonBody(r).get()["pushers"s]
    /*.get<immer::array<Pusher>>()*/;}
    else { return immer::array<Pusher>(  );}
    }



BaseJob::Query PostPusherJob::buildQuery(
)
{
BaseJob::Query _q;

return _q;
}

    BaseJob::Body PostPusherJob::buildBody(std::string pushkey, std::string kind, std::string appId, std::string appDisplayName, std::string deviceDisplayName, std::string lang, PusherData data, std::string profileTag, std::optional<bool> append)
      {
      // ignore unused param
      (void)(pushkey);(void)(kind);(void)(appId);(void)(appDisplayName);(void)(deviceDisplayName);(void)(lang);(void)(data);(void)(profileTag);(void)(append);
      
        json _data
        ;
        
            _data["pushkey"s] = pushkey;
          
            _data["kind"s] = kind;
          
            _data["app_id"s] = appId;
          
            _data["app_display_name"s] = appDisplayName;
          
            _data["device_display_name"s] = deviceDisplayName;
          
          
            addToJsonIfNeeded(_data, "profile_tag"s, profileTag);
            _data["lang"s] = lang;
          
            _data["data"s] = data;
          
          
            addToJsonIfNeeded(_data, "append"s, append);
        return BaseJob::JsonBody(_data);
        

      };

      

PostPusherJob::PostPusherJob(
        std::string serverUrl
        , std::string _accessToken
        ,
        std::string pushkey, std::string kind, std::string appId, std::string appDisplayName, std::string deviceDisplayName, std::string lang, PusherData data, std::string profileTag, std::optional<bool> append)
      : BaseJob(std::move(serverUrl),
          std::string("/_matrix/client/r0") + "/pushers/set",
          POST,
          _accessToken,
          ReturnType::Json,
            buildBody(pushkey, kind, appId, appDisplayName, deviceDisplayName, lang, data, profileTag, append)
              , buildQuery()
                )
        {
        }

          bool PostPusherJob::success(Response r)
          {
            return BaseJob::success(r)
            
              && isBodyJson(r.body)
          ;
          }


}
