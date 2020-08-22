/******************************************************************************
 * THIS FILE IS GENERATED - ANY EDITS WILL BE OVERWRITTEN
 */

#include "create_room.hpp"

namespace Kazv
{
  

    BaseJob::Body CreateRoomJob::buildBody(std::string visibility, std::string roomAliasName, std::string name, std::string topic, immer::array<std::string> invite, immer::array<Invite3pid> invite3pid, std::string roomVersion, JsonWrap creationContent, immer::array<StateEvent> initialState, std::string preset, std::optional<bool> isDirect, JsonWrap powerLevelContentOverride)
      {
      // ignore unused param
      (void)(visibility);(void)(roomAliasName);(void)(name);(void)(topic);(void)(invite);(void)(invite3pid);(void)(roomVersion);(void)(creationContent);(void)(initialState);(void)(preset);(void)(isDirect);(void)(powerLevelContentOverride);
      
        json _data
        ;
        
          
            addToJsonIfNeeded(_data, "visibility"s, visibility);
          
            addToJsonIfNeeded(_data, "room_alias_name"s, roomAliasName);
          
            addToJsonIfNeeded(_data, "name"s, name);
          
            addToJsonIfNeeded(_data, "topic"s, topic);
          
            addToJsonIfNeeded(_data, "invite"s, invite);
          
            addToJsonIfNeeded(_data, "invite_3pid"s, invite3pid);
          
            addToJsonIfNeeded(_data, "room_version"s, roomVersion);
          
            addToJsonIfNeeded(_data, "creation_content"s, creationContent);
          
            addToJsonIfNeeded(_data, "initial_state"s, initialState);
          
            addToJsonIfNeeded(_data, "preset"s, preset);
          
            addToJsonIfNeeded(_data, "is_direct"s, isDirect);
          
            addToJsonIfNeeded(_data, "power_level_content_override"s, powerLevelContentOverride);
        return BaseJob::JsonBody(_data);
        

      };

CreateRoomJob::CreateRoomJob(
        std::string serverUrl
        , std::string _accessToken
        ,
        std::string visibility, std::string roomAliasName, std::string name, std::string topic, immer::array<std::string> invite, immer::array<Invite3pid> invite3pid, std::string roomVersion, JsonWrap creationContent, immer::array<StateEvent> initialState, std::string preset, std::optional<bool> isDirect, JsonWrap powerLevelContentOverride)
      : BaseJob(std::move(serverUrl),
          std::string("/_matrix/client/r0") + "/createRoom",
          POST,
          _accessToken,
          ReturnType::Json,
            buildBody(visibility, roomAliasName, name, topic, invite, invite3pid, roomVersion, creationContent, initialState, preset, isDirect, powerLevelContentOverride)
      )
        {
        
        
          //addExpectedKey("room_id");
        }


    
    std::string CreateRoomJob::roomId(Response r)
    {
    if (jsonBody(r).get()
    .contains("room_id"s)) {
    return
    jsonBody(r).get()["room_id"s]
    /*.get<std::string>()*/;}
    else { return std::string(  );}
    }

}
