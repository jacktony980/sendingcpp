/******************************************************************************
 * THIS FILE IS GENERATED - ANY EDITS WILL BE OVERWRITTEN
 */

#pragma once

#include "basejob.hpp"


namespace Kazv {

/*! \brief Send a message event to the given room.
 *
 * This endpoint is used to send a message event to a room. Message events
 * allow access to historical events and pagination, making them suited
 * for "once-off" activity in a room.
 * 
 * The body of the request should be the content object of the event; the
 * fields in this object will vary depending on the type of event. See
 * `Room Events`_ for the m. event specification.
 */
class SendMessageJob : public BaseJob {
public:


// Construction/destruction

  /*! \brief Send a message event to the given room.
 *
    * \param roomId
    *   The room to send the event to.
    * 
    * \param eventType
    *   The type of event to send.
    * 
    * \param txnId
    *   The transaction ID for this event. Clients should generate an
    *   ID unique across requests with the same access token; it will be
    *   used by the server to ensure idempotency of requests.
    * 
    * \param body
    *   This endpoint is used to send a message event to a room. Message events
    *   allow access to historical events and pagination, making them suited
    *   for "once-off" activity in a room.
    *   
    *   The body of the request should be the content object of the event; the
    *   fields in this object will vary depending on the type of event. See
    *   `Room Events`_ for the m. event specification.
    */
    explicit SendMessageJob(std::string serverUrl
    , std::string _accessToken
      ,
        std::string roomId , std::string eventType , std::string txnId , JsonWrap body  = {});
    

    // Result properties
        
        

    
/// A unique identifier for the event.
static std::string eventId(Response r);

    

    static BaseJob::Body buildBody(std::string roomId, std::string eventType, std::string txnId, JsonWrap body);
      };

      } 
      namespace nlohmann
      {
      using namespace Kazv;
    
    }

    namespace Kazv
    {

} // namespace Kazv
