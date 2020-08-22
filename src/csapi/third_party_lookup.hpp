/******************************************************************************
 * THIS FILE IS GENERATED - ANY EDITS WILL BE OVERWRITTEN
 */

#pragma once

#include "basejob.hpp"
#include "csapi/../application-service/definitions/user.hpp"
#include "csapi/../application-service/definitions/location.hpp"
#include "csapi/../application-service/definitions/protocol.hpp"

namespace Kazv {

/*! \brief Retrieve metadata about all protocols that a homeserver supports.
 *
 * Fetches the overall metadata about protocols supported by the
 * homeserver. Includes both the available protocols and all fields
 * required for queries against each protocol.
 */
class GetProtocolsJob : public BaseJob {
public:


// Construction/destruction

    /// Retrieve metadata about all protocols that a homeserver supports.
    explicit GetProtocolsJob(std::string serverUrl
    , std::string _accessToken
      
        );


    // Result properties
        

/// The protocols supported by the homeserver.
    static immer::map<std::string, ThirdPartyProtocol> data(Response r)
    {
    return
    
      std::move(jsonBody(r).get()).get<immer::map<std::string, ThirdPartyProtocol>>()
    ;
    }
        

    

    static BaseJob::Body buildBody();
      };

      } 
      namespace nlohmann
      {
      using namespace Kazv;
    
    }

    namespace Kazv
    {

/*! \brief Retrieve metadata about a specific protocol that the homeserver supports.
 *
 * Fetches the metadata from the homeserver about a particular third party protocol.
 */
class GetProtocolMetadataJob : public BaseJob {
public:


// Construction/destruction

  /*! \brief Retrieve metadata about a specific protocol that the homeserver supports.
 *
    * \param protocol
    *   The name of the protocol.
    */
    explicit GetProtocolMetadataJob(std::string serverUrl
    , std::string _accessToken
      ,
        std::string protocol );


    // Result properties
        

/// The protocol was found and metadata returned.
    static ThirdPartyProtocol data(Response r)
    {
    return
    
      std::move(jsonBody(r).get()).get<ThirdPartyProtocol>()
    ;
    }
        

    

    static BaseJob::Body buildBody(std::string protocol);
      };

      } 
      namespace nlohmann
      {
      using namespace Kazv;
    
    }

    namespace Kazv
    {

/*! \brief Retrieve Matrix-side portals rooms leading to a third party location.
 *
 * Requesting this endpoint with a valid protocol name results in a list
 * of successful mapping results in a JSON array. Each result contains
 * objects to represent the Matrix room or rooms that represent a portal
 * to this third party network. Each has the Matrix room alias string,
 * an identifier for the particular third party network protocol, and an
 * object containing the network-specific fields that comprise this
 * identifier. It should attempt to canonicalise the identifier as much
 * as reasonably possible given the network type.
 */
class QueryLocationByProtocolJob : public BaseJob {
public:


// Construction/destruction

  /*! \brief Retrieve Matrix-side portals rooms leading to a third party location.
 *
    * \param protocol
    *   The protocol used to communicate to the third party network.
    * 
    * \param searchFields
    *   One or more custom fields to help identify the third party
    *   location.
    */
    explicit QueryLocationByProtocolJob(std::string serverUrl
    , std::string _accessToken
      ,
        std::string protocol , std::string searchFields  = {});


    // Result properties
        

/// At least one portal room was found.
    static immer::array<ThirdPartyLocation> data(Response r)
    {
    return
    
      std::move(jsonBody(r).get()).get<immer::array<ThirdPartyLocation>>()
    ;
    }
        

    static BaseJob::Query buildQuery(
    std::string searchFields);

    static BaseJob::Body buildBody(std::string protocol, std::string searchFields);
      };

      } 
      namespace nlohmann
      {
      using namespace Kazv;
    
    }

    namespace Kazv
    {

/*! \brief Retrieve the Matrix User ID of a corresponding third party user.
 *
 * Retrieve a Matrix User ID linked to a user on the third party service, given
 * a set of user parameters.
 */
class QueryUserByProtocolJob : public BaseJob {
public:


// Construction/destruction

  /*! \brief Retrieve the Matrix User ID of a corresponding third party user.
 *
    * \param protocol
    *   The name of the protocol.
    * 
    * \param fields
    *   One or more custom fields that are passed to the AS to help identify the user.
    */
    explicit QueryUserByProtocolJob(std::string serverUrl
    , std::string _accessToken
      ,
        std::string protocol , std::string fields  = {});


    // Result properties
        

/// The Matrix User IDs found with the given parameters.
    static immer::array<ThirdPartyUser> data(Response r)
    {
    return
    
      std::move(jsonBody(r).get()).get<immer::array<ThirdPartyUser>>()
    ;
    }
        

    static BaseJob::Query buildQuery(
    std::string fields);

    static BaseJob::Body buildBody(std::string protocol, std::string fields);
      };

      } 
      namespace nlohmann
      {
      using namespace Kazv;
    
    }

    namespace Kazv
    {

/*! \brief Reverse-lookup third party locations given a Matrix room alias.
 *
 * Retrieve an array of third party network locations from a Matrix room
 * alias.
 */
class QueryLocationByAliasJob : public BaseJob {
public:


// Construction/destruction

  /*! \brief Reverse-lookup third party locations given a Matrix room alias.
 *
    * \param alias
    *   The Matrix room alias to look up.
    */
    explicit QueryLocationByAliasJob(std::string serverUrl
    , std::string _accessToken
      ,
        std::string alias );


    // Result properties
        

/// All found third party locations.
    static immer::array<ThirdPartyLocation> data(Response r)
    {
    return
    
      std::move(jsonBody(r).get()).get<immer::array<ThirdPartyLocation>>()
    ;
    }
        

    static BaseJob::Query buildQuery(
    std::string alias);

    static BaseJob::Body buildBody(std::string alias);
      };

      } 
      namespace nlohmann
      {
      using namespace Kazv;
    
    }

    namespace Kazv
    {

/*! \brief Reverse-lookup third party users given a Matrix User ID.
 *
 * Retrieve an array of third party users from a Matrix User ID.
 */
class QueryUserByIDJob : public BaseJob {
public:


// Construction/destruction

  /*! \brief Reverse-lookup third party users given a Matrix User ID.
 *
    * \param userid
    *   The Matrix User ID to look up.
    */
    explicit QueryUserByIDJob(std::string serverUrl
    , std::string _accessToken
      ,
        std::string userid );


    // Result properties
        

/// An array of third party users.
    static immer::array<ThirdPartyUser> data(Response r)
    {
    return
    
      std::move(jsonBody(r).get()).get<immer::array<ThirdPartyUser>>()
    ;
    }
        

    static BaseJob::Query buildQuery(
    std::string userid);

    static BaseJob::Body buildBody(std::string userid);
      };

      } 
      namespace nlohmann
      {
      using namespace Kazv;
    
    }

    namespace Kazv
    {

} // namespace Kazv
