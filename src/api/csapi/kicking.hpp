/******************************************************************************
 * THIS FILE IS GENERATED - ANY EDITS WILL BE OVERWRITTEN
 */

#pragma once

#include "basejob.hpp"


namespace Kazv {

/*! \brief Kick a user from the room.
 *
 * Kick a user from the room.
 * 
 * The caller must have the required power level in order to perform this operation.
 * 
 * Kicking a user adjusts the target member's membership state to be ``leave`` with an
 * optional ``reason``. Like with other membership changes, a user can directly adjust
 * the target member's state by making a request to ``/rooms/<room id>/state/m.room.member/<user id>``.
 */
class KickJob : public BaseJob {
public:


          static constexpr auto needsAuth() {
          return true
            ;
              }


// Construction/destruction

  /*! \brief Kick a user from the room.
 *
    * \param roomId
    *   The room identifier (not alias) from which the user should be kicked.
    * 
    * \param userId
    *   The fully qualified user ID of the user being kicked.
    * 
    * \param reason
    *   The reason the user has been kicked. This will be supplied as the 
    *   ``reason`` on the target's updated `m.room.member`_ event.
    */
    explicit KickJob(std::string serverUrl
    , std::string _accessToken
      ,
        std::string roomId , std::string userId , std::string reason  = {});
    

    static BaseJob::Query buildQuery(
    );

      static BaseJob::Body buildBody(std::string roomId, std::string userId, std::string reason);

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
