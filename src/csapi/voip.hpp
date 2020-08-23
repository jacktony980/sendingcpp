/******************************************************************************
 * THIS FILE IS GENERATED - ANY EDITS WILL BE OVERWRITTEN
 */

#pragma once

#include "basejob.hpp"


namespace Kazv {

/*! \brief Obtain TURN server credentials.
 *
 * This API provides credentials for the client to use when initiating
 * calls.
 */
class GetTurnServerJob : public BaseJob {
public:


// Construction/destruction

    /// Obtain TURN server credentials.
    explicit GetTurnServerJob(std::string serverUrl
    , std::string _accessToken
      
        );


    // Result properties
        

/// The TURN server credentials.
    static JsonWrap data(Response r)
    {
    return
    
      std::move(jsonBody(r).get()).get<JsonWrap>()
    ;
    }
        

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
