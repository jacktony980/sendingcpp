/******************************************************************************
 * THIS FILE IS GENERATED - ANY EDITS WILL BE OVERWRITTEN
 */

#include <algorithm>

#include "tags.hpp"

namespace Kazv
{


BaseJob::Query GetRoomTagsJob::buildQuery(
)
{
BaseJob::Query _q;

return _q;
}

    BaseJob::Body GetRoomTagsJob::buildBody(std::string userId, std::string roomId)
      {
      // ignore unused param
      (void)(userId);(void)(roomId);
      
      
              return BaseJob::EmptyBody{};

      };

      

GetRoomTagsJob::GetRoomTagsJob(
        std::string serverUrl
        , std::string _accessToken
        ,
        std::string userId, std::string roomId)
      : BaseJob(std::move(serverUrl),
          std::string("/_matrix/client/r0") + "/user/" + userId + "/rooms/" + roomId + "/tags",
          GET,
          _accessToken,
          ReturnType::Json,
            buildBody(userId, roomId)
              , buildQuery()
                )
        {
        }

          bool GetRoomTagsJob::success(Response r)
          {
            return BaseJob::success(r)
            
              && isBodyJson(r.body)
          ;
          }


    
    immer::map<std::string, GetRoomTagsJob::Tag> GetRoomTagsJob::tags(Response r)
    {
    if (jsonBody(r).get()
    .contains("tags"s)) {
    return
    jsonBody(r).get()["tags"s]
    /*.get<immer::map<std::string, Tag>>()*/;}
    else { return immer::map<std::string, Tag>(  );}
    }



BaseJob::Query SetRoomTagJob::buildQuery(
)
{
BaseJob::Query _q;

return _q;
}

    BaseJob::Body SetRoomTagJob::buildBody(std::string userId, std::string roomId, std::string tag, std::optional<float> order, JsonWrap additionalProperties)
      {
      // ignore unused param
      (void)(userId);(void)(roomId);(void)(tag);(void)(order);(void)(additionalProperties);
      
        json _data
        ;
          addPropertyMapToJson(_data, additionalProperties);
          
            addToJsonIfNeeded(_data, "order"s, order);
        return BaseJob::JsonBody(_data);
        

      };

      

SetRoomTagJob::SetRoomTagJob(
        std::string serverUrl
        , std::string _accessToken
        ,
        std::string userId, std::string roomId, std::string tag, std::optional<float> order, JsonWrap additionalProperties)
      : BaseJob(std::move(serverUrl),
          std::string("/_matrix/client/r0") + "/user/" + userId + "/rooms/" + roomId + "/tags/" + tag,
          PUT,
          _accessToken,
          ReturnType::Json,
            buildBody(userId, roomId, tag, order, additionalProperties)
              , buildQuery()
                )
        {
        }

          bool SetRoomTagJob::success(Response r)
          {
            return BaseJob::success(r)
            
              && isBodyJson(r.body)
          ;
          }




BaseJob::Query DeleteRoomTagJob::buildQuery(
)
{
BaseJob::Query _q;

return _q;
}

    BaseJob::Body DeleteRoomTagJob::buildBody(std::string userId, std::string roomId, std::string tag)
      {
      // ignore unused param
      (void)(userId);(void)(roomId);(void)(tag);
      
      
              return BaseJob::EmptyBody{};

      };

      

DeleteRoomTagJob::DeleteRoomTagJob(
        std::string serverUrl
        , std::string _accessToken
        ,
        std::string userId, std::string roomId, std::string tag)
      : BaseJob(std::move(serverUrl),
          std::string("/_matrix/client/r0") + "/user/" + userId + "/rooms/" + roomId + "/tags/" + tag,
          DELETE,
          _accessToken,
          ReturnType::Json,
            buildBody(userId, roomId, tag)
              , buildQuery()
                )
        {
        }

          bool DeleteRoomTagJob::success(Response r)
          {
            return BaseJob::success(r)
            
              && isBodyJson(r.body)
          ;
          }


}
