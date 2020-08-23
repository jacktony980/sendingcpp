/******************************************************************************
 * THIS FILE IS GENERATED - ANY EDITS WILL BE OVERWRITTEN
 */

#pragma once

#include "basejob.hpp"
#include "csapi/definitions/sync_filter.hpp"

namespace Kazv {

/*! \brief Upload a new filter.
 *
 * Uploads a new filter definition to the homeserver.
 * Returns a filter ID that may be used in future requests to
 * restrict which events are returned to the client.
 */
class DefineFilterJob : public BaseJob {
public:


// Construction/destruction

  /*! \brief Upload a new filter.
 *
    * \param userId
    *   The id of the user uploading the filter. The access token must be authorized to make requests for this user id.
    * 
    * \param filter
    *   The filter to upload.
    */
    explicit DefineFilterJob(std::string serverUrl
    , std::string _accessToken
      ,
        std::string userId , Filter filter );
    

    // Result properties
        
        

    
/// The ID of the filter that was created. Cannot start
/// with a ``{`` as this character is used to determine
/// if the filter provided is inline JSON or a previously
/// declared filter by homeservers on some APIs.
static std::string filterId(Response r);

    static BaseJob::Query buildQuery(
    );

      static BaseJob::Body buildBody(std::string userId, Filter filter);

        static bool success(Response r);
        
      };

      } 
      namespace nlohmann
      {
      using namespace Kazv;
    
    }

    namespace Kazv
    {

/*! \brief Download a filter
 *
 */
class GetFilterJob : public BaseJob {
public:


// Construction/destruction

  /*! \brief Download a filter
 *
    * \param userId
    *   The user ID to download a filter for.
    * 
    * \param filterId
    *   The filter ID to download.
    */
    explicit GetFilterJob(std::string serverUrl
    , std::string _accessToken
      ,
        std::string userId , std::string filterId );


    // Result properties
        

/// The filter definition.
    static Filter data(Response r)
    {
    return
    
      std::move(jsonBody(r).get()).get<Filter>()
    ;
    }
        

    static BaseJob::Query buildQuery(
    );

      static BaseJob::Body buildBody(std::string userId, std::string filterId);

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
