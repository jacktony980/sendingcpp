/******************************************************************************
 * THIS FILE IS GENERATED - ANY EDITS WILL BE OVERWRITTEN
 */

#pragma once

#include "basejob.hpp"


namespace Kazv {

/*! \brief Gets information about the owner of an access token.
 *
 * Gets information about the owner of a given access token. 
 * 
 * Note that, as with the rest of the Client-Server API, 
 * Application Services may masquerade as users within their 
 * namespace by giving a ``user_id`` query parameter. In this 
 * situation, the server should verify that the given ``user_id``
 * is registered by the appservice, and return it in the response 
 * body.
 */
class GetTokenOwnerJob : public BaseJob {
public:


          static constexpr auto needsAuth() {
          return true
            ;
              }


// Construction/destruction

    /// Gets information about the owner of an access token.
    explicit GetTokenOwnerJob(std::string serverUrl
    , std::string _accessToken
      
        );


    // Result properties
        
        

    
/// The user id that owns the access token.
static std::string userId(Response r);

    static BaseJob::Query buildQuery(
    );

      static BaseJob::Body buildBody();

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
