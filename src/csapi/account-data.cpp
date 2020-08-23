/******************************************************************************
 * THIS FILE IS GENERATED - ANY EDITS WILL BE OVERWRITTEN
 */

#include <algorithm>

#include "account-data.hpp"

namespace Kazv
{


BaseJob::Query SetAccountDataJob::buildQuery(
)
{
BaseJob::Query _q;

return _q;
}

    BaseJob::Body SetAccountDataJob::buildBody(std::string userId, std::string type, JsonWrap content)
      {
      // ignore unused param
      (void)(userId);(void)(type);(void)(content);
        return 
          BaseJob::JsonBody(content);
      
          

      };

      

SetAccountDataJob::SetAccountDataJob(
        std::string serverUrl
        , std::string _accessToken
        ,
        std::string userId, std::string type, JsonWrap content)
      : BaseJob(std::move(serverUrl),
          std::string("/_matrix/client/r0") + "/user/" + userId + "/account_data/" + type,
          PUT,
          _accessToken,
          ReturnType::Json,
            buildBody(userId, type, content)
              , buildQuery()
                )
        {
        }

          bool SetAccountDataJob::success(Response r)
          {
            return BaseJob::success(r)
            
              && isBodyJson(r.body)
          ;
          }




BaseJob::Query GetAccountDataJob::buildQuery(
)
{
BaseJob::Query _q;

return _q;
}

    BaseJob::Body GetAccountDataJob::buildBody(std::string userId, std::string type)
      {
      // ignore unused param
      (void)(userId);(void)(type);
      
      
              return BaseJob::EmptyBody{};

      };

      

GetAccountDataJob::GetAccountDataJob(
        std::string serverUrl
        , std::string _accessToken
        ,
        std::string userId, std::string type)
      : BaseJob(std::move(serverUrl),
          std::string("/_matrix/client/r0") + "/user/" + userId + "/account_data/" + type,
          GET,
          _accessToken,
          ReturnType::Json,
            buildBody(userId, type)
              , buildQuery()
                )
        {
        }

          bool GetAccountDataJob::success(Response r)
          {
            return BaseJob::success(r)
            
              && isBodyJson(r.body)
          ;
          }




BaseJob::Query SetAccountDataPerRoomJob::buildQuery(
)
{
BaseJob::Query _q;

return _q;
}

    BaseJob::Body SetAccountDataPerRoomJob::buildBody(std::string userId, std::string roomId, std::string type, JsonWrap content)
      {
      // ignore unused param
      (void)(userId);(void)(roomId);(void)(type);(void)(content);
        return 
          BaseJob::JsonBody(content);
      
          

      };

      

SetAccountDataPerRoomJob::SetAccountDataPerRoomJob(
        std::string serverUrl
        , std::string _accessToken
        ,
        std::string userId, std::string roomId, std::string type, JsonWrap content)
      : BaseJob(std::move(serverUrl),
          std::string("/_matrix/client/r0") + "/user/" + userId + "/rooms/" + roomId + "/account_data/" + type,
          PUT,
          _accessToken,
          ReturnType::Json,
            buildBody(userId, roomId, type, content)
              , buildQuery()
                )
        {
        }

          bool SetAccountDataPerRoomJob::success(Response r)
          {
            return BaseJob::success(r)
            
              && isBodyJson(r.body)
          ;
          }




BaseJob::Query GetAccountDataPerRoomJob::buildQuery(
)
{
BaseJob::Query _q;

return _q;
}

    BaseJob::Body GetAccountDataPerRoomJob::buildBody(std::string userId, std::string roomId, std::string type)
      {
      // ignore unused param
      (void)(userId);(void)(roomId);(void)(type);
      
      
              return BaseJob::EmptyBody{};

      };

      

GetAccountDataPerRoomJob::GetAccountDataPerRoomJob(
        std::string serverUrl
        , std::string _accessToken
        ,
        std::string userId, std::string roomId, std::string type)
      : BaseJob(std::move(serverUrl),
          std::string("/_matrix/client/r0") + "/user/" + userId + "/rooms/" + roomId + "/account_data/" + type,
          GET,
          _accessToken,
          ReturnType::Json,
            buildBody(userId, roomId, type)
              , buildQuery()
                )
        {
        }

          bool GetAccountDataPerRoomJob::success(Response r)
          {
            return BaseJob::success(r)
            
              && isBodyJson(r.body)
          ;
          }


}
