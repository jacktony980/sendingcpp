/******************************************************************************
 * THIS FILE IS GENERATED - ANY EDITS WILL BE OVERWRITTEN
 */

#include "receipts.hpp"

namespace Kazv
{
  

    BaseJob::Body PostReceiptJob::buildBody(std::string roomId, std::string receiptType, std::string eventId, JsonWrap receipt)
      {
      // ignore unused param
      (void)(roomId);(void)(receiptType);(void)(eventId);(void)(receipt);
        return 
          BaseJob::JsonBody(receipt);
      
          

      };

PostReceiptJob::PostReceiptJob(
        std::string serverUrl
        , std::string _accessToken
        ,
        std::string roomId, std::string receiptType, std::string eventId, JsonWrap receipt)
      : BaseJob(std::move(serverUrl),
          std::string("/_matrix/client/r0") + "/rooms/" + roomId + "/receipt/" + receiptType + "/" + eventId,
          POST,
          _accessToken,
          ReturnType::Json,
            buildBody(roomId, receiptType, eventId, receipt)
      )
        {
        
        
        }


}
