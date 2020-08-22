/******************************************************************************
 * THIS FILE IS GENERATED - ANY EDITS WILL BE OVERWRITTEN
 */

#pragma once

#include "basejob.hpp"


namespace Kazv {

/*! \brief Upgrades a room to a new room version.
 *
 * Upgrades the given room to a particular room version.
 */
class UpgradeRoomJob : public BaseJob {
public:


// Construction/destruction

  /*! \brief Upgrades a room to a new room version.
 *
    * \param roomId
    *   The ID of the room to upgrade.
    * 
    * \param newVersion
    *   The new version for the room.
    */
    explicit UpgradeRoomJob(std::string serverUrl
    , std::string _accessToken
      ,
        std::string roomId , std::string newVersion );
    

    // Result properties
        
        

    
/// The ID of the new room.
static std::string replacementRoom(Response r);

    

    static BaseJob::Body buildBody(std::string roomId, std::string newVersion);
      };

      } 
      namespace nlohmann
      {
      using namespace Kazv;
    
    }

    namespace Kazv
    {

} // namespace Kazv
