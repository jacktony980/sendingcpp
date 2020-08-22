/******************************************************************************
 * THIS FILE IS GENERATED - ANY EDITS WILL BE OVERWRITTEN
 */

#pragma once

#include "basejob.hpp"
#include "csapi/definitions/auth_data.hpp"
#include "csapi/definitions/client_device.hpp"

namespace Kazv {

/*! \brief List registered devices for the current user
 *
 * Gets information about all devices for the current user.
 */
class GetDevicesJob : public BaseJob {
public:


// Construction/destruction

    /// List registered devices for the current user
    explicit GetDevicesJob(std::string serverUrl
    , std::string _accessToken
      
        );


    // Result properties
        
        

    
/// A list of all registered devices for this user.
static immer::array<Device> devices(Response r);

    

    static BaseJob::Body buildBody();
      };

      } 
      namespace nlohmann
      {
      using namespace Kazv;
    
    }

    namespace Kazv
    {

/*! \brief Get a single device
 *
 * Gets information on a single device, by device id.
 */
class GetDeviceJob : public BaseJob {
public:


// Construction/destruction

  /*! \brief Get a single device
 *
    * \param deviceId
    *   The device to retrieve.
    */
    explicit GetDeviceJob(std::string serverUrl
    , std::string _accessToken
      ,
        std::string deviceId );


    // Result properties
        

/// Device information
    static Device data(Response r)
    {
    return
    
      std::move(jsonBody(r).get()).get<Device>()
    ;
    }
        

    

    static BaseJob::Body buildBody(std::string deviceId);
      };

      } 
      namespace nlohmann
      {
      using namespace Kazv;
    
    }

    namespace Kazv
    {

/*! \brief Update a device
 *
 * Updates the metadata on the given device.
 */
class UpdateDeviceJob : public BaseJob {
public:


// Construction/destruction

  /*! \brief Update a device
 *
    * \param deviceId
    *   The device to update.
    * 
    * \param displayName
    *   The new display name for this device. If not given, the
    *   display name is unchanged.
    */
    explicit UpdateDeviceJob(std::string serverUrl
    , std::string _accessToken
      ,
        std::string deviceId , std::string displayName  = {});
    

    

    static BaseJob::Body buildBody(std::string deviceId, std::string displayName);
      };

      } 
      namespace nlohmann
      {
      using namespace Kazv;
    
    }

    namespace Kazv
    {

/*! \brief Delete a device
 *
 * This API endpoint uses the `User-Interactive Authentication API`_.
 * 
 * Deletes the given device, and invalidates any access token associated with it.
 */
class DeleteDeviceJob : public BaseJob {
public:


// Construction/destruction

  /*! \brief Delete a device
 *
    * \param deviceId
    *   The device to delete.
    * 
    * \param auth
    *   Additional authentication information for the
    *   user-interactive authentication API.
    */
    explicit DeleteDeviceJob(std::string serverUrl
    , std::string _accessToken
      ,
        std::string deviceId , std::optional<AuthenticationData> auth  = std::nullopt);
    

    

    static BaseJob::Body buildBody(std::string deviceId, std::optional<AuthenticationData> auth);
      };

      } 
      namespace nlohmann
      {
      using namespace Kazv;
    
    }

    namespace Kazv
    {

/*! \brief Bulk deletion of devices
 *
 * This API endpoint uses the `User-Interactive Authentication API`_.
 * 
 * Deletes the given devices, and invalidates any access token associated with them.
 */
class DeleteDevicesJob : public BaseJob {
public:


// Construction/destruction

  /*! \brief Bulk deletion of devices
 *
    * \param devices
    *   The list of device IDs to delete.
    * 
    * \param auth
    *   Additional authentication information for the
    *   user-interactive authentication API.
    */
    explicit DeleteDevicesJob(std::string serverUrl
    , std::string _accessToken
      ,
        immer::array<std::string> devices , std::optional<AuthenticationData> auth  = std::nullopt);
    

    

    static BaseJob::Body buildBody(immer::array<std::string> devices, std::optional<AuthenticationData> auth);
      };

      } 
      namespace nlohmann
      {
      using namespace Kazv;
    
    }

    namespace Kazv
    {

} // namespace Kazv
