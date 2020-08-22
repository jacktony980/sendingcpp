/******************************************************************************
 * THIS FILE IS GENERATED - ANY EDITS WILL BE OVERWRITTEN
 */

#pragma once

#include "types.hpp"
#include "csapi/definitions/push_rule.hpp"

namespace Kazv {

struct PushRuleset
{       

    
    immer::array<PushRule> content;

    
    immer::array<PushRule> override;

    
    immer::array<PushRule> room;

    
    immer::array<PushRule> sender;

    
    immer::array<PushRule> underride;
};

}
namespace nlohmann
{
using namespace Kazv;
template<>
struct adl_serializer<PushRuleset> {
  static void to_json(json& jo, const PushRuleset &pod)
  {
  
  
    
    addToJsonIfNeeded(jo, "content"s, pod.content);
    
    addToJsonIfNeeded(jo, "override"s, pod.override);
    
    addToJsonIfNeeded(jo, "room"s, pod.room);
    
    addToJsonIfNeeded(jo, "sender"s, pod.sender);
    
    addToJsonIfNeeded(jo, "underride"s, pod.underride);
  }
  static void from_json(const json &jo, PushRuleset& result)
  {
  
    result.content = jo.at("content"s);
    result.override = jo.at("override"s);
    result.room = jo.at("room"s);
    result.sender = jo.at("sender"s);
    result.underride = jo.at("underride"s);
  
  }
};
    }

    namespace Kazv
    {
} // namespace Kazv
