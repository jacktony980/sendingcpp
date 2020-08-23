/******************************************************************************
 * THIS FILE IS GENERATED - ANY EDITS WILL BE OVERWRITTEN
 */

#pragma once

#include "basejob.hpp"


namespace Kazv {

/*! \brief Listen on the event stream.
 *
 * This will listen for new events related to a particular room and return
 * them to the caller. This will block until an event is received, or until
 * the ``timeout`` is reached.
 * 
 * This API is the same as the normal ``/events`` endpoint, but can be
 * called by users who have not joined the room.
 * 
 * Note that the normal ``/events`` endpoint has been deprecated. This
 * API will also be deprecated at some point, but its replacement is not
 * yet known.
 */
class PeekEventsJob : public BaseJob {
public:


// Construction/destruction

  /*! \brief Listen on the event stream.
 *
    * \param from
    *   The token to stream from. This token is either from a previous
    *   request to this API or from the initial sync API.
    * 
    * \param timeout
    *   The maximum time in milliseconds to wait for an event.
    * 
    * \param roomId
    *   The room ID for which events should be returned.
    */
    explicit PeekEventsJob(std::string serverUrl
    , std::string _accessToken
      ,
        std::string from  = {}, std::optional<int> timeout  = std::nullopt, std::string roomId  = {});


    // Result properties
        
        

    
/// A token which correlates to the first value in ``chunk``. This
/// is usually the same token supplied to ``from=``.
static std::string start(Response r);

    
/// A token which correlates to the last value in ``chunk``. This
/// token should be used in the next request to ``/events``.
static std::string end(Response r);

    
/// An array of events.
static EventList chunk(Response r);

    static BaseJob::Query buildQuery(
    std::string from, std::optional<int> timeout, std::string roomId);

      static BaseJob::Body buildBody(std::string from, std::optional<int> timeout, std::string roomId);

        static bool success(Response r);
        
      };

      } 
      namespace nlohmann
      {
      using namespace Kazv;
    
    }

    namespace Kazv
    {

} // namespace Kazv
