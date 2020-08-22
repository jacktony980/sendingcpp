/******************************************************************************
 * THIS FILE IS GENERATED - ANY EDITS WILL BE OVERWRITTEN
 */

#include "account-data.hpp"

namespace Kazv
{
  

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
      )
        {
        
        
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
      )
        {
        
        
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
      )
        {
        
        
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
      )
        {
        
        
        }


}
