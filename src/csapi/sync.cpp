/******************************************************************************
 * THIS FILE IS GENERATED - ANY EDITS WILL BE OVERWRITTEN
 */

#include "sync.hpp"

namespace Kazv
{

BaseJob::Query SyncJob::buildQuery(
std::string filter, std::string since, std::optional<bool> fullState, std::string setPresence, std::optional<int> timeout)
{
BaseJob::Query _q;
  
    addToQueryIfNeeded(_q, "filter"s, filter);
  
    addToQueryIfNeeded(_q, "since"s, since);
  
    addToQueryIfNeeded(_q, "full_state"s, fullState);
  
    addToQueryIfNeeded(_q, "set_presence"s, setPresence);
  
    addToQueryIfNeeded(_q, "timeout"s, timeout);
return _q;
}

    BaseJob::Body SyncJob::buildBody(std::string filter, std::string since, std::optional<bool> fullState, std::string setPresence, std::optional<int> timeout)
      {
      // ignore unused param
      (void)(filter);(void)(since);(void)(fullState);(void)(setPresence);(void)(timeout);
      
      
              return BaseJob::EmptyBody{};

      };

SyncJob::SyncJob(
        std::string serverUrl
        , std::string _accessToken
        ,
        std::string filter, std::string since, std::optional<bool> fullState, std::string setPresence, std::optional<int> timeout)
      : BaseJob(std::move(serverUrl),
          std::string("/_matrix/client/r0") + "/sync",
          GET,
          _accessToken,
          ReturnType::Json,
            buildBody(filter, since, fullState, setPresence, timeout)
      , buildQuery(filter, since, fullState, setPresence, timeout))
        {
        
        
          //addExpectedKey("next_batch");
        }


    
    std::string SyncJob::nextBatch(Response r)
    {
    if (jsonBody(r).get()
    .contains("next_batch"s)) {
    return
    jsonBody(r).get()["next_batch"s]
    /*.get<std::string>()*/;}
    else { return std::string(  );}
    }

    
    std::optional<SyncJob::Rooms> SyncJob::rooms(Response r)
    {
    if (jsonBody(r).get()
    .contains("rooms"s)) {
    return
    jsonBody(r).get()["rooms"s]
    /*.get<Rooms>()*/;}
    else { return std::optional<Rooms>(  );}
    }

    
    std::optional<EventBatch> SyncJob::presence(Response r)
    {
    if (jsonBody(r).get()
    .contains("presence"s)) {
    return
    jsonBody(r).get()["presence"s]
    /*.get<EventBatch>()*/;}
    else { return std::optional<EventBatch>(  );}
    }

    
    std::optional<EventBatch> SyncJob::accountData(Response r)
    {
    if (jsonBody(r).get()
    .contains("account_data"s)) {
    return
    jsonBody(r).get()["account_data"s]
    /*.get<EventBatch>()*/;}
    else { return std::optional<EventBatch>(  );}
    }

    
    JsonWrap SyncJob::toDevice(Response r)
    {
    if (jsonBody(r).get()
    .contains("to_device"s)) {
    return
    jsonBody(r).get()["to_device"s]
    /*.get<JsonWrap>()*/;}
    else { return JsonWrap(  );}
    }

    
    JsonWrap SyncJob::deviceLists(Response r)
    {
    if (jsonBody(r).get()
    .contains("device_lists"s)) {
    return
    jsonBody(r).get()["device_lists"s]
    /*.get<JsonWrap>()*/;}
    else { return JsonWrap(  );}
    }

    
    immer::map<std::string, int> SyncJob::deviceOneTimeKeysCount(Response r)
    {
    if (jsonBody(r).get()
    .contains("device_one_time_keys_count"s)) {
    return
    jsonBody(r).get()["device_one_time_keys_count"s]
    /*.get<immer::map<std::string, int>>()*/;}
    else { return immer::map<std::string, int>(  );}
    }

}
