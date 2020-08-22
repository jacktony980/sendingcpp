/******************************************************************************
 * THIS FILE IS GENERATED - ANY EDITS WILL BE OVERWRITTEN
 */

#pragma once

#include "basejob.hpp"
#include "csapi/definitions/device_keys.hpp"

namespace Kazv {

/*! \brief Upload end-to-end encryption keys.
 *
 * Publishes end-to-end encryption keys for the device.
 */
class UploadKeysJob : public BaseJob {
public:


// Construction/destruction

  /*! \brief Upload end-to-end encryption keys.
 *
    * \param deviceKeys
    *   Identity keys for the device. May be absent if no new
    *   identity keys are required.
    * 
    * \param oneTimeKeys
    *   One-time public keys for "pre-key" messages.  The names of
    *   the properties should be in the format
    *   ``<algorithm>:<key_id>``. The format of the key is determined
    *   by the `key algorithm <#key-algorithms>`_.
    *   
    *   May be absent if no new one-time keys are required.
    */
    explicit UploadKeysJob(std::string serverUrl
    , std::string _accessToken
      ,
        std::optional<DeviceKeys> deviceKeys  = std::nullopt, immer::map<std::string, Variant> oneTimeKeys  = {});
    

    // Result properties
        
        

    
/// For each key algorithm, the number of unclaimed one-time keys
/// of that type currently held on the server for this device.
static immer::map<std::string, int> oneTimeKeyCounts(Response r);

    

    static BaseJob::Body buildBody(std::optional<DeviceKeys> deviceKeys, immer::map<std::string, Variant> oneTimeKeys);
      };

      } 
      namespace nlohmann
      {
      using namespace Kazv;
    
    }

    namespace Kazv
    {

/*! \brief Download device identity keys.
 *
 * Returns the current devices and identity keys for the given users.
 */
class QueryKeysJob : public BaseJob {
public:
  // Inner data structures

/// Additional data added to the device key information
/// by intermediate servers, and not covered by the
/// signatures.
    struct UnsignedDeviceInfo
        {
/// The display name which the user set on the device.
          std::string deviceDisplayName;
        
        };

/// Returns the current devices and identity keys for the given users.
    struct DeviceInformation :
      DeviceKeys
        {
/// Additional data added to the device key information
/// by intermediate servers, and not covered by the
/// signatures.
          std::optional<UnsignedDeviceInfo> unsignedData;
        
        };


// Construction/destruction

  /*! \brief Download device identity keys.
 *
    * \param deviceKeys
    *   The keys to be downloaded. A map from user ID, to a list of
    *   device IDs, or to an empty list to indicate all devices for the
    *   corresponding user.
    * 
    * \param timeout
    *   The time (in milliseconds) to wait when downloading keys from
    *   remote servers. 10 seconds is the recommended default.
    * 
    * \param token
    *   If the client is fetching keys as a result of a device update received
    *   in a sync request, this should be the 'since' token of that sync request,
    *   or any later sync token. This allows the server to ensure its response
    *   contains the keys advertised by the notification in that sync.
    */
    explicit QueryKeysJob(std::string serverUrl
    , std::string _accessToken
      ,
        immer::map<std::string, immer::array<std::string>> deviceKeys , std::optional<int> timeout  = std::nullopt, std::string token  = {});
    

    // Result properties
        
        

    
/// If any remote homeservers could not be reached, they are
/// recorded here. The names of the properties are the names of
/// the unreachable servers.
/// 
/// If the homeserver could be reached, but the user or device
/// was unknown, no failure is recorded. Instead, the corresponding
/// user or device is missing from the ``device_keys`` result.
static immer::map<std::string, JsonWrap> failures(Response r);

    
/// Information on the queried devices. A map from user ID, to a
/// map from device ID to device information.  For each device,
/// the information returned will be the same as uploaded via
/// ``/keys/upload``, with the addition of an ``unsigned``
/// property.
static immer::map<std::string, immer::map<std::string, DeviceInformation>> deviceKeys(Response r);

    

    static BaseJob::Body buildBody(immer::map<std::string, immer::array<std::string>> deviceKeys, std::optional<int> timeout, std::string token);
      };

      } 
      namespace nlohmann
      {
      using namespace Kazv;
      template<>
      struct adl_serializer<QueryKeysJob::UnsignedDeviceInfo> {

  static void from_json(const json &jo, QueryKeysJob::UnsignedDeviceInfo& result)
  {
  
    result.deviceDisplayName = jo.at("device_display_name"s);
  
  }
};
      template<>
      struct adl_serializer<QueryKeysJob::DeviceInformation> {

  static void from_json(const json &jo, QueryKeysJob::DeviceInformation& result)
  {
    static_cast<DeviceKeys &>(result) = jo;
    //nlohmann::from_json(jo, static_cast<const DeviceKeys &>(result));
    result.unsignedData = jo.at("unsigned"s);
  
  }
};
    }

    namespace Kazv
    {

/*! \brief Claim one-time encryption keys.
 *
 * Claims one-time keys for use in pre-key messages.
 */
class ClaimKeysJob : public BaseJob {
public:


// Construction/destruction

  /*! \brief Claim one-time encryption keys.
 *
    * \param oneTimeKeys
    *   The keys to be claimed. A map from user ID, to a map from
    *   device ID to algorithm name.
    * 
    * \param timeout
    *   The time (in milliseconds) to wait when downloading keys from
    *   remote servers. 10 seconds is the recommended default.
    */
    explicit ClaimKeysJob(std::string serverUrl
    , std::string _accessToken
      ,
        immer::map<std::string, immer::map<std::string, std::string>> oneTimeKeys , std::optional<int> timeout  = std::nullopt);
    

    // Result properties
        
        

    
/// If any remote homeservers could not be reached, they are
/// recorded here. The names of the properties are the names of
/// the unreachable servers.
/// 
/// If the homeserver could be reached, but the user or device
/// was unknown, no failure is recorded. Instead, the corresponding
/// user or device is missing from the ``one_time_keys`` result.
static immer::map<std::string, JsonWrap> failures(Response r);

    
/// One-time keys for the queried devices. A map from user ID, to a
/// map from devices to a map from ``<algorithm>:<key_id>`` to the key object.
/// 
/// See the `key algorithms <#key-algorithms>`_ section for information
/// on the Key Object format.
static immer::map<std::string, immer::map<std::string, Variant>> oneTimeKeys(Response r);

    

    static BaseJob::Body buildBody(immer::map<std::string, immer::map<std::string, std::string>> oneTimeKeys, std::optional<int> timeout);
      };

      } 
      namespace nlohmann
      {
      using namespace Kazv;
    
    }

    namespace Kazv
    {

/*! \brief Query users with recent device key updates.
 *
 * Gets a list of users who have updated their device identity keys since a
 * previous sync token.
 * 
 * The server should include in the results any users who:
 * 
 * * currently share a room with the calling user (ie, both users have
 *   membership state ``join``); *and*
 * * added new device identity keys or removed an existing device with
 *   identity keys, between ``from`` and ``to``.
 */
class GetKeysChangesJob : public BaseJob {
public:


// Construction/destruction

  /*! \brief Query users with recent device key updates.
 *
    * \param from
    *   The desired start point of the list. Should be the ``next_batch`` field
    *   from a response to an earlier call to |/sync|. Users who have not
    *   uploaded new device identity keys since this point, nor deleted
    *   existing devices with identity keys since then, will be excluded
    *   from the results.
    * 
    * \param to
    *   The desired end point of the list. Should be the ``next_batch``
    *   field from a recent call to |/sync| - typically the most recent
    *   such call. This may be used by the server as a hint to check its
    *   caches are up to date.
    */
    explicit GetKeysChangesJob(std::string serverUrl
    , std::string _accessToken
      ,
        std::string from , std::string to );


    // Result properties
        
        

    
/// The Matrix User IDs of all users who updated their device
/// identity keys.
static immer::array<std::string> changed(Response r);

    
/// The Matrix User IDs of all users who may have left all
/// the end-to-end encrypted rooms they previously shared
/// with the user.
static immer::array<std::string> left(Response r);

    static BaseJob::Query buildQuery(
    std::string from, std::string to);

    static BaseJob::Body buildBody(std::string from, std::string to);
      };

      } 
      namespace nlohmann
      {
      using namespace Kazv;
    
    }

    namespace Kazv
    {

} // namespace Kazv
