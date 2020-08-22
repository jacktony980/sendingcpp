/******************************************************************************
 * THIS FILE IS GENERATED - ANY EDITS WILL BE OVERWRITTEN
 */

#pragma once

#include "basejob.hpp"
#include "csapi/definitions/event-schemas/schema/m.room.member.hpp"

namespace Kazv {

/*! \brief Listen on the event stream.
 *
 * This will listen for new events and return them to the caller. This will
 * block until an event is received, or until the ``timeout`` is reached.
 * 
 * This endpoint was deprecated in r0 of this specification. Clients
 * should instead call the |/sync|_ API with a ``since`` parameter. See
 * the `migration guide
 * <https://matrix.org/docs/guides/client-server-migrating-from-v1.html#deprecated-endpoints>`_.
 */
class GetEventsJob : public BaseJob {
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
    */
    explicit GetEventsJob(std::string serverUrl
    , std::string _accessToken
      ,
        std::string from  = {}, std::optional<int> timeout  = std::nullopt);


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
    std::string from, std::optional<int> timeout);

    static BaseJob::Body buildBody(std::string from, std::optional<int> timeout);
      };

      } 
      namespace nlohmann
      {
      using namespace Kazv;
    
    }

    namespace Kazv
    {

/*! \brief Get the user's current state.
 *
 * This returns the full state for this user, with an optional limit on the
 * number of messages per room to return.
 * 
 * This endpoint was deprecated in r0 of this specification. Clients
 * should instead call the |/sync|_ API with no ``since`` parameter. See
 * the `migration guide
 * <https://matrix.org/docs/guides/client-server-migrating-from-v1.html#deprecated-endpoints>`_.
 */
class InitialSyncJob : public BaseJob {
public:
  // Inner data structures

/// The pagination chunk for this room.
    struct PaginationChunk
        {
/// A token which correlates to the first value in ``chunk``.
/// Used for pagination.
          std::string start;
/// A token which correlates to the last value in ``chunk``.
/// Used for pagination.
          std::string end;
/// If the user is a member of the room this will be a
/// list of the most recent messages for this room. If
/// the user has left the room this will be the
/// messages that preceeded them leaving. This array
/// will consist of at most ``limit`` elements.
          EventList chunk;
        
        };

/// This returns the full state for this user, with an optional limit on the
/// number of messages per room to return.
/// 
/// This endpoint was deprecated in r0 of this specification. Clients
/// should instead call the |/sync|_ API with no ``since`` parameter. See
/// the `migration guide
/// <https://matrix.org/docs/guides/client-server-migrating-from-v1.html#deprecated-endpoints>`_.
    struct RoomInfo
        {
/// The ID of this room.
          std::string roomId;
/// The user's membership state in this room.
          std::string membership;
/// The invite event if ``membership`` is ``invite``
          std::optional<TheCurrentMembershipStateOfAUserInTheRoom> invite;
/// The pagination chunk for this room.
          std::optional<PaginationChunk> messages;
/// If the user is a member of the room this will be the
/// current state of the room as a list of events. If the
/// user has left the room this will be the state of the
/// room when they left it.
          EventList state;
/// Whether this room is visible to the ``/publicRooms`` API
/// or not."
          std::string visibility;
/// The private data that this user has attached to
/// this room.
          EventList accountData;
        
        };


// Construction/destruction

  /*! \brief Get the user's current state.
 *
    * \param limit
    *   The maximum number of messages to return for each room.
    * 
    * \param archived
    *   Whether to include rooms that the user has left. If ``false`` then
    *   only rooms that the user has been invited to or has joined are
    *   included. If set to ``true`` then rooms that the user has left are
    *   included as well. By default this is ``false``.
    */
    explicit InitialSyncJob(std::string serverUrl
    , std::string _accessToken
      ,
        std::optional<int> limit  = std::nullopt, std::optional<bool> archived  = std::nullopt);


    // Result properties
        
        

    
/// A token which correlates to the last value in ``chunk``. This
/// token should be used with the ``/events`` API to listen for new
/// events.
static std::string end(Response r);

    
/// A list of presence events.
static EventList presence(Response r);

    
/// This returns the full state for this user, with an optional limit on the
/// number of messages per room to return.
/// 
/// This endpoint was deprecated in r0 of this specification. Clients
/// should instead call the |/sync|_ API with no ``since`` parameter. See
/// the `migration guide
/// <https://matrix.org/docs/guides/client-server-migrating-from-v1.html#deprecated-endpoints>`_.
static immer::array<RoomInfo> rooms(Response r);

    
/// The global private data created by this user.
static EventList accountData(Response r);

    static BaseJob::Query buildQuery(
    std::optional<int> limit, std::optional<bool> archived);

    static BaseJob::Body buildBody(std::optional<int> limit, std::optional<bool> archived);
      };

      } 
      namespace nlohmann
      {
      using namespace Kazv;
      template<>
      struct adl_serializer<InitialSyncJob::PaginationChunk> {

  static void from_json(const json &jo, InitialSyncJob::PaginationChunk& result)
  {
  
    result.start = jo.at("start"s);
    result.end = jo.at("end"s);
    result.chunk = jo.at("chunk"s);
  
  }
};
      template<>
      struct adl_serializer<InitialSyncJob::RoomInfo> {

  static void from_json(const json &jo, InitialSyncJob::RoomInfo& result)
  {
  
    result.roomId = jo.at("room_id"s);
    result.membership = jo.at("membership"s);
    result.invite = jo.at("invite"s);
    result.messages = jo.at("messages"s);
    result.state = jo.at("state"s);
    result.visibility = jo.at("visibility"s);
    result.accountData = jo.at("account_data"s);
  
  }
};
    }

    namespace Kazv
    {

/*! \brief Get a single event by event ID.
 *
 * Get a single event based on ``event_id``. You must have permission to
 * retrieve this event e.g. by being a member in the room for this event.
 * 
 * This endpoint was deprecated in r0 of this specification. Clients
 * should instead call the |/rooms/{roomId}/event/{eventId}|_ API
 * or the |/rooms/{roomId}/context/{eventId}|_ API.
 */
class GetOneEventJob : public BaseJob {
public:


// Construction/destruction

  /*! \brief Get a single event by event ID.
 *
    * \param eventId
    *   The event ID to get.
    */
    explicit GetOneEventJob(std::string serverUrl
    , std::string _accessToken
      ,
        std::string eventId );


    // Result properties
        

/// The full event.
    static JsonWrap data(Response r)
    {
    return
    
      std::move(jsonBody(r).get()).get<JsonWrap>()
    ;
    }
        

    

    static BaseJob::Body buildBody(std::string eventId);
      };

      } 
      namespace nlohmann
      {
      using namespace Kazv;
    
    }

    namespace Kazv
    {

} // namespace Kazv
