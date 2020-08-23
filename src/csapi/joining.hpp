/******************************************************************************
 * THIS FILE IS GENERATED - ANY EDITS WILL BE OVERWRITTEN
 */

#pragma once

#include "basejob.hpp"
#include "csapi/definitions/third_party_signed.hpp"

namespace Kazv {

/*! \brief Start the requesting user participating in a particular room.
 *
 * *Note that this API requires a room ID, not alias.*
 * ``/join/{roomIdOrAlias}`` *exists if you have a room alias.*
 * 
 * This API starts a user participating in a particular room, if that user
 * is allowed to participate in that room. After this call, the client is
 * allowed to see all current state events in the room, and all subsequent
 * events associated with the room until the user leaves the room.
 * 
 * After a user has joined a room, the room will appear as an entry in the
 * response of the |/initialSync|_ and |/sync|_ APIs.
 */
class JoinRoomByIdJob : public BaseJob {
public:


// Construction/destruction

  /*! \brief Start the requesting user participating in a particular room.
 *
    * \param roomId
    *   The room identifier (not alias) to join.
    * 
    * \param thirdPartySigned
    *   If supplied, the homeserver must verify that it matches a pending
    *   ``m.room.third_party_invite`` event in the room, and perform
    *   key validity checking if required by the event.
    */
    explicit JoinRoomByIdJob(std::string serverUrl
    , std::string _accessToken
      ,
        std::string roomId , std::optional<ThirdPartySigned> thirdPartySigned  = std::nullopt);
    

    // Result properties
        
        

    
/// The joined room ID.
static std::string roomId(Response r);

    static BaseJob::Query buildQuery(
    );

      static BaseJob::Body buildBody(std::string roomId, std::optional<ThirdPartySigned> thirdPartySigned);

        static bool success(Response r);
        
      };

      } 
      namespace nlohmann
      {
      using namespace Kazv;
    
    }

    namespace Kazv
    {

/*! \brief Start the requesting user participating in a particular room.
 *
 * *Note that this API takes either a room ID or alias, unlike* ``/room/{roomId}/join``.
 * 
 * This API starts a user participating in a particular room, if that user
 * is allowed to participate in that room. After this call, the client is
 * allowed to see all current state events in the room, and all subsequent
 * events associated with the room until the user leaves the room.
 * 
 * After a user has joined a room, the room will appear as an entry in the
 * response of the |/initialSync|_ and |/sync|_ APIs.
 */
class JoinRoomJob : public BaseJob {
public:


// Construction/destruction

  /*! \brief Start the requesting user participating in a particular room.
 *
    * \param roomIdOrAlias
    *   The room identifier or alias to join.
    * 
    * \param serverName
    *   The servers to attempt to join the room through. One of the servers
    *   must be participating in the room.
    * 
    * \param thirdPartySigned
    *   If a ``third_party_signed`` was supplied, the homeserver must verify
    *   that it matches a pending ``m.room.third_party_invite`` event in the
    *   room, and perform key validity checking if required by the event.
    */
    explicit JoinRoomJob(std::string serverUrl
    , std::string _accessToken
      ,
        std::string roomIdOrAlias , immer::array<std::string> serverName  = {}, std::optional<ThirdPartySigned> thirdPartySigned  = std::nullopt);
    

    // Result properties
        
        

    
/// The joined room ID.
static std::string roomId(Response r);

    static BaseJob::Query buildQuery(
    immer::array<std::string> serverName);

      static BaseJob::Body buildBody(std::string roomIdOrAlias, immer::array<std::string> serverName, std::optional<ThirdPartySigned> thirdPartySigned);

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
