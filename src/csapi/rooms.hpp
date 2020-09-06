/******************************************************************************
 * THIS FILE IS GENERATED - ANY EDITS WILL BE OVERWRITTEN
 */

#pragma once

#include "basejob.hpp"
#include "csapi/definitions/event-schemas/schema/m.room.member.hpp"

namespace Kazv {

/*! \brief Get a single event by event ID.
 *
 * Get a single event based on ``roomId/eventId``. You must have permission to
 * retrieve this event e.g. by being a member in the room for this event.
 */
class GetOneRoomEventJob : public BaseJob {
public:


          static constexpr auto needsAuth() {
          return true
            ;
              }


// Construction/destruction

  /*! \brief Get a single event by event ID.
 *
    * \param roomId
    *   The ID of the room the event is in.
    * 
    * \param eventId
    *   The event ID to get.
    */
    explicit GetOneRoomEventJob(std::string serverUrl
    , std::string _accessToken
      ,
        std::string roomId , std::string eventId );


    // Result properties
        

/// The full event.
    static JsonWrap data(Response r)
    {
    return
    
      std::move(jsonBody(r).get()).get<JsonWrap>()
    ;
    }
        

    static BaseJob::Query buildQuery(
    );

      static BaseJob::Body buildBody(std::string roomId, std::string eventId);

        static bool success(Response r);
        
      };

      } 
      namespace nlohmann
      {
      using namespace Kazv;
    
    }

    namespace Kazv
    {

/*! \brief Get the state identified by the type and key.
 *
 * .. For backwards compatibility with older links...
 * .. _`get-matrix-client-r0-rooms-roomid-state-eventtype`:
 * 
 * Looks up the contents of a state event in a room. If the user is
 * joined to the room then the state is taken from the current
 * state of the room. If the user has left the room then the state is
 * taken from the state of the room when they left.
 */
class GetRoomStateWithKeyJob : public BaseJob {
public:


          static constexpr auto needsAuth() {
          return true
            ;
              }


// Construction/destruction

  /*! \brief Get the state identified by the type and key.
 *
    * \param roomId
    *   The room to look up the state in.
    * 
    * \param eventType
    *   The type of state to look up.
    * 
    * \param stateKey
    *   The key of the state to look up. Defaults to an empty string. When
    *   an empty string, the trailing slash on this endpoint is optional.
    */
    explicit GetRoomStateWithKeyJob(std::string serverUrl
    , std::string _accessToken
      ,
        std::string roomId , std::string eventType , std::string stateKey );


    static BaseJob::Query buildQuery(
    );

      static BaseJob::Body buildBody(std::string roomId, std::string eventType, std::string stateKey);

        static bool success(Response r);
        
      };

      } 
      namespace nlohmann
      {
      using namespace Kazv;
    
    }

    namespace Kazv
    {

/*! \brief Get all state events in the current state of a room.
 *
 * Get the state events for the current state of a room.
 */
class GetRoomStateJob : public BaseJob {
public:


          static constexpr auto needsAuth() {
          return true
            ;
              }


// Construction/destruction

  /*! \brief Get all state events in the current state of a room.
 *
    * \param roomId
    *   The room to look up the state for.
    */
    explicit GetRoomStateJob(std::string serverUrl
    , std::string _accessToken
      ,
        std::string roomId );


    // Result properties
        

/// The current state of the room
    static EventList data(Response r)
    {
    return
    
      std::move(jsonBody(r).get()).get<EventList>()
    ;
    }
        

    static BaseJob::Query buildQuery(
    );

      static BaseJob::Body buildBody(std::string roomId);

        static bool success(Response r);
        
      };

      } 
      namespace nlohmann
      {
      using namespace Kazv;
    
    }

    namespace Kazv
    {

/*! \brief Get the m.room.member events for the room.
 *
 * Get the list of members for this room.
 */
class GetMembersByRoomJob : public BaseJob {
public:


          static constexpr auto needsAuth() {
          return true
            ;
              }


// Construction/destruction

  /*! \brief Get the m.room.member events for the room.
 *
    * \param roomId
    *   The room to get the member events for.
    * 
    * \param at
    *   The point in time (pagination token) to return members for in the room.
    *   This token can be obtained from a ``prev_batch`` token returned for
    *   each room by the sync API. Defaults to the current state of the room,
    *   as determined by the server.
    * 
    * \param membership
    *   The kind of membership to filter for. Defaults to no filtering if
    *   unspecified. When specified alongside ``not_membership``, the two
    *   parameters create an 'or' condition: either the membership *is*
    *   the same as ``membership`` **or** *is not* the same as ``not_membership``.
    * 
    * \param notMembership
    *   The kind of membership to exclude from the results. Defaults to no
    *   filtering if unspecified.
    */
    explicit GetMembersByRoomJob(std::string serverUrl
    , std::string _accessToken
      ,
        std::string roomId , std::string at  = {}, std::string membership  = {}, std::string notMembership  = {});


    // Result properties
        
        

    
/// Get the list of members for this room.
static immer::array<TheCurrentMembershipStateOfAUserInTheRoom> chunk(Response r);

    static BaseJob::Query buildQuery(
    std::string at, std::string membership, std::string notMembership);

      static BaseJob::Body buildBody(std::string roomId, std::string at, std::string membership, std::string notMembership);

        static bool success(Response r);
        
      };

      } 
      namespace nlohmann
      {
      using namespace Kazv;
    
    }

    namespace Kazv
    {

/*! \brief Gets the list of currently joined users and their profile data.
 *
 * This API returns a map of MXIDs to member info objects for members of the room. The current user must be in the room for it to work, unless it is an Application Service in which case any of the AS's users must be in the room. This API is primarily for Application Services and should be faster to respond than ``/members`` as it can be implemented more efficiently on the server.
 */
class GetJoinedMembersByRoomJob : public BaseJob {
public:
  // Inner data structures

/// This API returns a map of MXIDs to member info objects for members of the room. The current user must be in the room for it to work, unless it is an Application Service in which case any of the AS's users must be in the room. This API is primarily for Application Services and should be faster to respond than ``/members`` as it can be implemented more efficiently on the server.
    struct RoomMember
        {
/// The display name of the user this object is representing.
          std::string displayName;
/// The mxc avatar url of the user this object is representing.
          std::string avatarUrl;
        
        };


          static constexpr auto needsAuth() {
          return true
            ;
              }


// Construction/destruction

  /*! \brief Gets the list of currently joined users and their profile data.
 *
    * \param roomId
    *   The room to get the members of.
    */
    explicit GetJoinedMembersByRoomJob(std::string serverUrl
    , std::string _accessToken
      ,
        std::string roomId );


    // Result properties
        
        

    
/// A map from user ID to a RoomMember object.
static immer::map<std::string, RoomMember> joined(Response r);

    static BaseJob::Query buildQuery(
    );

      static BaseJob::Body buildBody(std::string roomId);

        static bool success(Response r);
        
      };

      } 
      namespace nlohmann
      {
      using namespace Kazv;
      template<>
      struct adl_serializer<GetJoinedMembersByRoomJob::RoomMember> {

  static void from_json(const json &jo, GetJoinedMembersByRoomJob::RoomMember& result)
  {
  
    if (jo.contains("display_name"s)) {
      result.displayName = jo.at("display_name"s);
    }
    if (jo.contains("avatar_url"s)) {
      result.avatarUrl = jo.at("avatar_url"s);
    }
  
  }
};
    }

    namespace Kazv
    {

} // namespace Kazv
