/******************************************************************************
 * THIS FILE IS GENERATED - ANY EDITS WILL BE OVERWRITTEN
 */

#pragma once

#include "basejob.hpp"
#include "csapi/definitions/public_rooms_response.hpp"

namespace Kazv {

/*! \brief Gets the visibility of a room in the directory
 *
 * Gets the visibility of a given room on the server's public room directory.
 */
class GetRoomVisibilityOnDirectoryJob : public BaseJob {
public:


          static constexpr auto needsAuth() {
          return 
            false;
              }


// Construction/destruction

  /*! \brief Gets the visibility of a room in the directory
 *
    * \param roomId
    *   The room ID.
    */
    explicit GetRoomVisibilityOnDirectoryJob(std::string serverUrl
    
      ,
        std::string roomId );


    // Result properties
        
        

    
/// The visibility of the room in the directory.
static std::string visibility(Response r);

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

/*! \brief Sets the visibility of a room in the room directory
 *
 * Sets the visibility of a given room in the server's public room
 * directory.
 * 
 * Servers may choose to implement additional access control checks
 * here, for instance that room visibility can only be changed by 
 * the room creator or a server administrator.
 */
class SetRoomVisibilityOnDirectoryJob : public BaseJob {
public:


          static constexpr auto needsAuth() {
          return true
            ;
              }


// Construction/destruction

  /*! \brief Sets the visibility of a room in the room directory
 *
    * \param roomId
    *   The room ID.
    * 
    * \param visibility
    *   The new visibility setting for the room. 
    *   Defaults to 'public'.
    */
    explicit SetRoomVisibilityOnDirectoryJob(std::string serverUrl
    , std::string _accessToken
      ,
        std::string roomId , std::string visibility  = {});
    

    static BaseJob::Query buildQuery(
    );

      static BaseJob::Body buildBody(std::string roomId, std::string visibility);

        static bool success(Response r);
        
      };

      } 
      namespace nlohmann
      {
      using namespace Kazv;
    
    }

    namespace Kazv
    {

/*! \brief Lists the public rooms on the server.
 *
 * Lists the public rooms on the server.
 * 
 * This API returns paginated responses. The rooms are ordered by the number
 * of joined members, with the largest rooms first.
 */
class GetPublicRoomsJob : public BaseJob {
public:


          static constexpr auto needsAuth() {
          return 
            false;
              }


// Construction/destruction

  /*! \brief Lists the public rooms on the server.
 *
    * \param limit
    *   Limit the number of results returned.
    * 
    * \param since
    *   A pagination token from a previous request, allowing clients to
    *   get the next (or previous) batch of rooms.
    *   The direction of pagination is specified solely by which token
    *   is supplied, rather than via an explicit flag.
    * 
    * \param server
    *   The server to fetch the public room lists from. Defaults to the
    *   local server.
    */
    explicit GetPublicRoomsJob(std::string serverUrl
    
      ,
        std::optional<int> limit  = std::nullopt, std::string since  = {}, std::string server  = {});


    // Result properties
        
        

    
/// A paginated chunk of public rooms.
static immer::array<PublicRoomsChunk> chunk(Response r);

    
/// A pagination token for the response. The absence of this token
/// means there are no more results to fetch and the client should
/// stop paginating.
static std::string nextBatch(Response r);

    
/// A pagination token that allows fetching previous results. The
/// absence of this token means there are no results before this
/// batch, i.e. this is the first batch.
static std::string prevBatch(Response r);

    
/// An estimate on the total number of public rooms, if the
/// server has an estimate.
static std::optional<int> totalRoomCountEstimate(Response r);

    static BaseJob::Query buildQuery(
    std::optional<int> limit, std::string since, std::string server);

      static BaseJob::Body buildBody(std::optional<int> limit, std::string since, std::string server);

        static bool success(Response r);
        
      };

      } 
      namespace nlohmann
      {
      using namespace Kazv;
    
    }

    namespace Kazv
    {

/*! \brief Lists the public rooms on the server with optional filter.
 *
 * Lists the public rooms on the server, with optional filter.
 * 
 * This API returns paginated responses. The rooms are ordered by the number
 * of joined members, with the largest rooms first.
 */
class QueryPublicRoomsJob : public BaseJob {
public:
  // Inner data structures

/// Filter to apply to the results.
    struct Filter
        {
/// A string to search for in the room metadata, e.g. name,
/// topic, canonical alias etc. (Optional).
          std::string genericSearchTerm;
        
        };


          static constexpr auto needsAuth() {
          return true
            ;
              }


// Construction/destruction

  /*! \brief Lists the public rooms on the server with optional filter.
 *
    * \param server
    *   The server to fetch the public room lists from. Defaults to the
    *   local server.
    * 
    * \param limit
    *   Limit the number of results returned.
    * 
    * \param since
    *   A pagination token from a previous request, allowing clients
    *   to get the next (or previous) batch of rooms.  The direction
    *   of pagination is specified solely by which token is supplied,
    *   rather than via an explicit flag.
    * 
    * \param filter
    *   Filter to apply to the results.
    * 
    * \param includeAllNetworks
    *   Whether or not to include all known networks/protocols from
    *   application services on the homeserver. Defaults to false.
    * 
    * \param thirdPartyInstanceId
    *   The specific third party network/protocol to request from the
    *   homeserver. Can only be used if ``include_all_networks`` is false.
    */
    explicit QueryPublicRoomsJob(std::string serverUrl
    , std::string _accessToken
      ,
        std::string server  = {}, std::optional<int> limit  = std::nullopt, std::string since  = {}, std::optional<Filter> filter  = std::nullopt, std::optional<bool> includeAllNetworks  = std::nullopt, std::string thirdPartyInstanceId  = {});
    

    // Result properties
        
        

    
/// A paginated chunk of public rooms.
static immer::array<PublicRoomsChunk> chunk(Response r);

    
/// A pagination token for the response. The absence of this token
/// means there are no more results to fetch and the client should
/// stop paginating.
static std::string nextBatch(Response r);

    
/// A pagination token that allows fetching previous results. The
/// absence of this token means there are no results before this
/// batch, i.e. this is the first batch.
static std::string prevBatch(Response r);

    
/// An estimate on the total number of public rooms, if the
/// server has an estimate.
static std::optional<int> totalRoomCountEstimate(Response r);

    static BaseJob::Query buildQuery(
    std::string server);

      static BaseJob::Body buildBody(std::string server, std::optional<int> limit, std::string since, std::optional<Filter> filter, std::optional<bool> includeAllNetworks, std::string thirdPartyInstanceId);

        static bool success(Response r);
        
      };

      } 
      namespace nlohmann
      {
      using namespace Kazv;
      template<>
      struct adl_serializer<QueryPublicRoomsJob::Filter> {
  static void to_json(json& jo, const QueryPublicRoomsJob::Filter &pod)
  {
  
  
    
    addToJsonIfNeeded(jo, "generic_search_term"s, pod.genericSearchTerm);
  }

};
    }

    namespace Kazv
    {

} // namespace Kazv
