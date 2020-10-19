/******************************************************************************
 * THIS FILE IS GENERATED - ANY EDITS WILL BE OVERWRITTEN
 */

#include <algorithm>

#include "third_party_membership.hpp"

namespace Kazv
{


BaseJob::Query InviteBy3PIDJob::buildQuery(
)
{
BaseJob::Query _q;

return _q;
}

    BaseJob::Body InviteBy3PIDJob::buildBody(std::string roomId, std::string idServer, std::string idAccessToken, std::string medium, std::string address)
      {
      // ignore unused param
      (void)(roomId);(void)(idServer);(void)(idAccessToken);(void)(medium);(void)(address);
      
        json _data
        ;
        
            _data["id_server"s] = idServer;
          
            _data["id_access_token"s] = idAccessToken;
          
            _data["medium"s] = medium;
          
            _data["address"s] = address;
          
        return BaseJob::JsonBody(_data);
        

      };

      

InviteBy3PIDJob::InviteBy3PIDJob(
        std::string serverUrl
        , std::string _accessToken
        ,
        std::string roomId, std::string idServer, std::string idAccessToken, std::string medium, std::string address)
      : BaseJob(std::move(serverUrl),
          std::string("/_matrix/client/r0") + "/rooms/" + roomId + "/invite",
          POST,
          _accessToken,
          ReturnType::Json,
            buildBody(roomId, idServer, idAccessToken, medium, address)
              , buildQuery()
                )
        {
        }

          bool InviteBy3PIDJob::success(Response r)
          {
            return BaseJob::success(r)
            
              && isBodyJson(r.body)
          ;
          }


}
