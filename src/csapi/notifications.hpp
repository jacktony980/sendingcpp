/******************************************************************************
 * THIS FILE IS GENERATED - ANY EDITS WILL BE OVERWRITTEN
 */

#pragma once

#include "basejob.hpp"


namespace Kazv {

/*! \brief Gets a list of events that the user has been notified about
 *
 * This API is used to paginate through the list of events that the
 * user has been, or would have been notified about.
 */
class GetNotificationsJob : public BaseJob {
public:
  // Inner data structures

/// This API is used to paginate through the list of events that the
/// user has been, or would have been notified about.
    struct Notification
        {
/// The action(s) to perform when the conditions for this rule are met.
/// See `Push Rules: API`_.
          immer::array<Variant> actions;
/// The Event object for the event that triggered the notification.
          JsonWrap event;
/// The profile tag of the rule that matched this event.
          std::string profileTag;
/// Indicates whether the user has sent a read receipt indicating
/// that they have read this message.
          bool read;
/// The ID of the room in which the event was posted.
          std::string roomId;
/// The unix timestamp at which the event notification was sent,
/// in milliseconds.
          int ts;
        
        };


// Construction/destruction

  /*! \brief Gets a list of events that the user has been notified about
 *
    * \param from
    *   Pagination token given to retrieve the next set of events.
    * 
    * \param limit
    *   Limit on the number of events to return in this request.
    * 
    * \param only
    *   Allows basic filtering of events returned. Supply ``highlight``
    *   to return only events where the notification had the highlight
    *   tweak set.
    */
    explicit GetNotificationsJob(std::string serverUrl
    , std::string _accessToken
      ,
        std::string from  = {}, std::optional<int> limit  = std::nullopt, std::string only  = {});


    // Result properties
        
        

    
/// The token to supply in the ``from`` param of the next
/// ``/notifications`` request in order to request more
/// events. If this is absent, there are no more results.
static std::string nextToken(Response r);

    
/// The list of events that triggered notifications.
static immer::array<Notification> notifications(Response r);

    static BaseJob::Query buildQuery(
    std::string from, std::optional<int> limit, std::string only);

      static BaseJob::Body buildBody(std::string from, std::optional<int> limit, std::string only);

        static bool success(Response r);
        
      };

      } 
      namespace nlohmann
      {
      using namespace Kazv;
      template<>
      struct adl_serializer<GetNotificationsJob::Notification> {

  static void from_json(const json &jo, GetNotificationsJob::Notification& result)
  {
  
    result.actions = jo.at("actions"s);
    result.event = jo.at("event"s);
    result.profileTag = jo.at("profile_tag"s);
    result.read = jo.at("read"s);
    result.roomId = jo.at("room_id"s);
    result.ts = jo.at("ts"s);
  
  }
};
    }

    namespace Kazv
    {

} // namespace Kazv
