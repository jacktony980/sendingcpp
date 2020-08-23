/******************************************************************************
 * THIS FILE IS GENERATED - ANY EDITS WILL BE OVERWRITTEN
 */

#pragma once

#include "basejob.hpp"


namespace Kazv {

/*! \brief Lists the user's current rooms.
 *
 * This API returns a list of the user's current rooms.
 */
class GetJoinedRoomsJob : public BaseJob {
public:


// Construction/destruction

    /// Lists the user's current rooms.
    explicit GetJoinedRoomsJob(std::string serverUrl
    , std::string _accessToken
      
        );


    // Result properties
        
        

    
/// The ID of each room in which the user has ``joined`` membership.
static immer::array<std::string> joinedRooms(Response r);

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
