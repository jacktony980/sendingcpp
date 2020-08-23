/******************************************************************************
 * THIS FILE IS GENERATED - ANY EDITS WILL BE OVERWRITTEN
 */

#pragma once

#include "basejob.hpp"


namespace Kazv {

/*! \brief Set the user's display name.
 *
 * This API sets the given user's display name. You must have permission to
 * set this user's display name, e.g. you need to have their ``access_token``.
 */
class SetDisplayNameJob : public BaseJob {
public:


// Construction/destruction

  /*! \brief Set the user's display name.
 *
    * \param userId
    *   The user whose display name to set.
    * 
    * \param displayname
    *   The new display name for this user.
    */
    explicit SetDisplayNameJob(std::string serverUrl
    , std::string _accessToken
      ,
        std::string userId , std::string displayname  = {});
    

    static BaseJob::Query buildQuery(
    );

      static BaseJob::Body buildBody(std::string userId, std::string displayname);

        static bool success(Response r);
        
      };

      } 
      namespace nlohmann
      {
      using namespace Kazv;
    
    }

    namespace Kazv
    {

/*! \brief Get the user's display name.
 *
 * Get the user's display name. This API may be used to fetch the user's
 * own displayname or to query the name of other users; either locally or
 * on remote homeservers.
 */
class GetDisplayNameJob : public BaseJob {
public:


// Construction/destruction

  /*! \brief Get the user's display name.
 *
    * \param userId
    *   The user whose display name to get.
    */
    explicit GetDisplayNameJob(std::string serverUrl
    
      ,
        std::string userId );


    // Result properties
        
        

    
/// The user's display name if they have set one, otherwise not present.
static std::string displayname(Response r);

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

/*! \brief Set the user's avatar URL.
 *
 * This API sets the given user's avatar URL. You must have permission to
 * set this user's avatar URL, e.g. you need to have their ``access_token``.
 */
class SetAvatarUrlJob : public BaseJob {
public:


// Construction/destruction

  /*! \brief Set the user's avatar URL.
 *
    * \param userId
    *   The user whose avatar URL to set.
    * 
    * \param avatarUrl
    *   The new avatar URL for this user.
    */
    explicit SetAvatarUrlJob(std::string serverUrl
    , std::string _accessToken
      ,
        std::string userId , std::string avatarUrl  = {});
    

    static BaseJob::Query buildQuery(
    );

      static BaseJob::Body buildBody(std::string userId, std::string avatarUrl);

        static bool success(Response r);
        
      };

      } 
      namespace nlohmann
      {
      using namespace Kazv;
    
    }

    namespace Kazv
    {

/*! \brief Get the user's avatar URL.
 *
 * Get the user's avatar URL. This API may be used to fetch the user's
 * own avatar URL or to query the URL of other users; either locally or
 * on remote homeservers.
 */
class GetAvatarUrlJob : public BaseJob {
public:


// Construction/destruction

  /*! \brief Get the user's avatar URL.
 *
    * \param userId
    *   The user whose avatar URL to get.
    */
    explicit GetAvatarUrlJob(std::string serverUrl
    
      ,
        std::string userId );


    // Result properties
        
        

    
/// The user's avatar URL if they have set one, otherwise not present.
static std::string avatarUrl(Response r);

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

/*! \brief Get this user's profile information.
 *
 * Get the combined profile information for this user. This API may be used
 * to fetch the user's own profile information or other users; either
 * locally or on remote homeservers. This API may return keys which are not
 * limited to ``displayname`` or ``avatar_url``.
 */
class GetUserProfileJob : public BaseJob {
public:


// Construction/destruction

  /*! \brief Get this user's profile information.
 *
    * \param userId
    *   The user whose profile information to get.
    */
    explicit GetUserProfileJob(std::string serverUrl
    
      ,
        std::string userId );


    // Result properties
        
        

    
/// The user's avatar URL if they have set one, otherwise not present.
static std::string avatarUrl(Response r);

    
/// The user's display name if they have set one, otherwise not present.
static std::string displayname(Response r);

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
