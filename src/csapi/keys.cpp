/******************************************************************************
 * THIS FILE IS GENERATED - ANY EDITS WILL BE OVERWRITTEN
 */

#include "keys.hpp"

namespace Kazv
{
  

    BaseJob::Body UploadKeysJob::buildBody(std::optional<DeviceKeys> deviceKeys, immer::map<std::string, Variant> oneTimeKeys)
      {
      // ignore unused param
      (void)(deviceKeys);(void)(oneTimeKeys);
      
        json _data
        ;
        
          
            addToJsonIfNeeded(_data, "device_keys"s, deviceKeys);
          
            addToJsonIfNeeded(_data, "one_time_keys"s, oneTimeKeys);
        return BaseJob::JsonBody(_data);
        

      };

UploadKeysJob::UploadKeysJob(
        std::string serverUrl
        , std::string _accessToken
        ,
        std::optional<DeviceKeys> deviceKeys, immer::map<std::string, Variant> oneTimeKeys)
      : BaseJob(std::move(serverUrl),
          std::string("/_matrix/client/r0") + "/keys/upload",
          POST,
          _accessToken,
          ReturnType::Json,
            buildBody(deviceKeys, oneTimeKeys)
      )
        {
        
        
          //addExpectedKey("one_time_key_counts");
        }


    
    immer::map<std::string, int> UploadKeysJob::oneTimeKeyCounts(Response r)
    {
    if (jsonBody(r).get()
    .contains("one_time_key_counts"s)) {
    return
    jsonBody(r).get()["one_time_key_counts"s]
    /*.get<immer::map<std::string, int>>()*/;}
    else { return immer::map<std::string, int>(  );}
    }

  

    BaseJob::Body QueryKeysJob::buildBody(immer::map<std::string, immer::array<std::string>> deviceKeys, std::optional<int> timeout, std::string token)
      {
      // ignore unused param
      (void)(deviceKeys);(void)(timeout);(void)(token);
      
        json _data
        ;
        
          
            addToJsonIfNeeded(_data, "timeout"s, timeout);
            _data["device_keys"s] = deviceKeys;
          
          
            addToJsonIfNeeded(_data, "token"s, token);
        return BaseJob::JsonBody(_data);
        

      };

QueryKeysJob::QueryKeysJob(
        std::string serverUrl
        , std::string _accessToken
        ,
        immer::map<std::string, immer::array<std::string>> deviceKeys, std::optional<int> timeout, std::string token)
      : BaseJob(std::move(serverUrl),
          std::string("/_matrix/client/r0") + "/keys/query",
          POST,
          _accessToken,
          ReturnType::Json,
            buildBody(deviceKeys, timeout, token)
      )
        {
        
        
        }


    
    immer::map<std::string, JsonWrap> QueryKeysJob::failures(Response r)
    {
    if (jsonBody(r).get()
    .contains("failures"s)) {
    return
    jsonBody(r).get()["failures"s]
    /*.get<immer::map<std::string, JsonWrap>>()*/;}
    else { return immer::map<std::string, JsonWrap>(  );}
    }

    
    immer::map<std::string, immer::map<std::string, QueryKeysJob::DeviceInformation>> QueryKeysJob::deviceKeys(Response r)
    {
    if (jsonBody(r).get()
    .contains("device_keys"s)) {
    return
    jsonBody(r).get()["device_keys"s]
    /*.get<immer::map<std::string, immer::map<std::string, DeviceInformation>>>()*/;}
    else { return immer::map<std::string, immer::map<std::string, DeviceInformation>>(  );}
    }

  

    BaseJob::Body ClaimKeysJob::buildBody(immer::map<std::string, immer::map<std::string, std::string>> oneTimeKeys, std::optional<int> timeout)
      {
      // ignore unused param
      (void)(oneTimeKeys);(void)(timeout);
      
        json _data
        ;
        
          
            addToJsonIfNeeded(_data, "timeout"s, timeout);
            _data["one_time_keys"s] = oneTimeKeys;
          
        return BaseJob::JsonBody(_data);
        

      };

ClaimKeysJob::ClaimKeysJob(
        std::string serverUrl
        , std::string _accessToken
        ,
        immer::map<std::string, immer::map<std::string, std::string>> oneTimeKeys, std::optional<int> timeout)
      : BaseJob(std::move(serverUrl),
          std::string("/_matrix/client/r0") + "/keys/claim",
          POST,
          _accessToken,
          ReturnType::Json,
            buildBody(oneTimeKeys, timeout)
      )
        {
        
        
          //addExpectedKey("one_time_keys");
        }


    
    immer::map<std::string, JsonWrap> ClaimKeysJob::failures(Response r)
    {
    if (jsonBody(r).get()
    .contains("failures"s)) {
    return
    jsonBody(r).get()["failures"s]
    /*.get<immer::map<std::string, JsonWrap>>()*/;}
    else { return immer::map<std::string, JsonWrap>(  );}
    }

    
    immer::map<std::string, immer::map<std::string, Variant>> ClaimKeysJob::oneTimeKeys(Response r)
    {
    if (jsonBody(r).get()
    .contains("one_time_keys"s)) {
    return
    jsonBody(r).get()["one_time_keys"s]
    /*.get<immer::map<std::string, immer::map<std::string, Variant>>>()*/;}
    else { return immer::map<std::string, immer::map<std::string, Variant>>(  );}
    }


BaseJob::Query GetKeysChangesJob::buildQuery(
std::string from, std::string to)
{
BaseJob::Query _q;
    addToQuery(_q, "from"s, from);
  
    addToQuery(_q, "to"s, to);
  
return _q;
}

    BaseJob::Body GetKeysChangesJob::buildBody(std::string from, std::string to)
      {
      // ignore unused param
      (void)(from);(void)(to);
      
      
              return BaseJob::EmptyBody{};

      };

GetKeysChangesJob::GetKeysChangesJob(
        std::string serverUrl
        , std::string _accessToken
        ,
        std::string from, std::string to)
      : BaseJob(std::move(serverUrl),
          std::string("/_matrix/client/r0") + "/keys/changes",
          GET,
          _accessToken,
          ReturnType::Json,
            buildBody(from, to)
      , buildQuery(from, to))
        {
        
        
        }


    
    immer::array<std::string> GetKeysChangesJob::changed(Response r)
    {
    if (jsonBody(r).get()
    .contains("changed"s)) {
    return
    jsonBody(r).get()["changed"s]
    /*.get<immer::array<std::string>>()*/;}
    else { return immer::array<std::string>(  );}
    }

    
    immer::array<std::string> GetKeysChangesJob::left(Response r)
    {
    if (jsonBody(r).get()
    .contains("left"s)) {
    return
    jsonBody(r).get()["left"s]
    /*.get<immer::array<std::string>>()*/;}
    else { return immer::array<std::string>(  );}
    }

}
