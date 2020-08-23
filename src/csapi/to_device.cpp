/******************************************************************************
 * THIS FILE IS GENERATED - ANY EDITS WILL BE OVERWRITTEN
 */

#include <algorithm>

#include "to_device.hpp"

namespace Kazv
{


BaseJob::Query SendToDeviceJob::buildQuery(
)
{
BaseJob::Query _q;

return _q;
}

    BaseJob::Body SendToDeviceJob::buildBody(std::string eventType, std::string txnId, immer::map<std::string, immer::map<std::string, JsonWrap>> messages)
      {
      // ignore unused param
      (void)(eventType);(void)(txnId);(void)(messages);
      
        json _data
        ;
        
          
            addToJsonIfNeeded(_data, "messages"s, messages);
        return BaseJob::JsonBody(_data);
        

      };

      

SendToDeviceJob::SendToDeviceJob(
        std::string serverUrl
        , std::string _accessToken
        ,
        std::string eventType, std::string txnId, immer::map<std::string, immer::map<std::string, JsonWrap>> messages)
      : BaseJob(std::move(serverUrl),
          std::string("/_matrix/client/r0") + "/sendToDevice/" + eventType + "/" + txnId,
          PUT,
          _accessToken,
          ReturnType::Json,
            buildBody(eventType, txnId, messages)
              , buildQuery()
                )
        {
        }

          bool SendToDeviceJob::success(Response r)
          {
            return BaseJob::success(r)
            
              && isBodyJson(r.body)
          ;
          }


}
