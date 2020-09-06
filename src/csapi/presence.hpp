/******************************************************************************
 * THIS FILE IS GENERATED - ANY EDITS WILL BE OVERWRITTEN
 */

#pragma once

#include "basejob.hpp"


namespace Kazv {

/*! \brief Update this user's presence state.
 *
 * This API sets the given user's presence state. When setting the status,
 * the activity time is updated to reflect that activity; the client does
 * not need to specify the ``last_active_ago`` field. You cannot set the
 * presence state of another user.
 */
class SetPresenceJob : public BaseJob {
public:


          static constexpr auto needsAuth() {
          return true
            ;
              }


// Construction/destruction

  /*! \brief Update this user's presence state.
 *
    * \param userId
    *   The user whose presence state to update.
    * 
    * \param presence
    *   The new presence state.
    * 
    * \param statusMsg
    *   The status message to attach to this state.
    */
    explicit SetPresenceJob(std::string serverUrl
    , std::string _accessToken
      ,
        std::string userId , std::string presence , std::string statusMsg  = {});
    

    static BaseJob::Query buildQuery(
    );

      static BaseJob::Body buildBody(std::string userId, std::string presence, std::string statusMsg);

        static bool success(Response r);
        
      };

      } 
      namespace nlohmann
      {
      using namespace Kazv;
    
    }

    namespace Kazv
    {

/*! \brief Get this user's presence state.
 *
 * Get the given user's presence state.
 */
class GetPresenceJob : public BaseJob {
public:


          static constexpr auto needsAuth() {
          return true
            ;
              }


// Construction/destruction

  /*! \brief Get this user's presence state.
 *
    * \param userId
    *   The user whose presence state to get.
    */
    explicit GetPresenceJob(std::string serverUrl
    , std::string _accessToken
      ,
        std::string userId );


    // Result properties
        
        

    
/// This user's presence.
static std::string presence(Response r);

    
/// The length of time in milliseconds since an action was performed
/// by this user.
static std::optional<int> lastActiveAgo(Response r);

    
/// The state message for this user if one was set.
static Variant statusMsg(Response r);

    
/// Whether the user is currently active
static std::optional<bool> currentlyActive(Response r);

    static BaseJob::Query buildQuery(
    );

      static BaseJob::Body buildBody(std::string userId);

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
