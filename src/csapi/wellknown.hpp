/******************************************************************************
 * THIS FILE IS GENERATED - ANY EDITS WILL BE OVERWRITTEN
 */

#pragma once

#include "basejob.hpp"
#include "csapi/definitions/wellknown/full.hpp"

namespace Kazv {

/*! \brief Gets Matrix server discovery information about the domain.
 *
 * Gets discovery information about the domain. The file may include
 * additional keys, which MUST follow the Java package naming convention,
 * e.g. ``com.example.myapp.property``. This ensures property names are
 * suitably namespaced for each application and reduces the risk of
 * clashes.
 * 
 * Note that this endpoint is not necessarily handled by the homeserver,
 * but by another webserver, to be used for discovering the homeserver URL.
 */
class GetWellknownJob : public BaseJob {
public:


// Construction/destruction

    /// Gets Matrix server discovery information about the domain.
    explicit GetWellknownJob(std::string serverUrl
    
      
        );


    // Result properties
        

/// Server discovery information.
    static DiscoveryInformation data(Response r)
    {
    return
    
      std::move(jsonBody(r).get()).get<DiscoveryInformation>()
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
