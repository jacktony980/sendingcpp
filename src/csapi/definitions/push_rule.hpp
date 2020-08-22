/******************************************************************************
 * THIS FILE IS GENERATED - ANY EDITS WILL BE OVERWRITTEN
 */

#pragma once

#include "types.hpp"
#include "csapi/definitions/push_condition.hpp"

namespace Kazv {

struct PushRule
{       

/// The actions to perform when this rule is matched.
    immer::array<Variant> actions;

/// Whether this is a default rule, or has been set explicitly.
    bool isDefault;

/// Whether the push rule is enabled or not.
    bool enabled;

/// The ID of this rule.
    std::string ruleId;

/// The conditions that must hold true for an event in order for a rule to be
/// applied to an event. A rule with no conditions always matches. Only
/// applicable to ``underride`` and ``override`` rules.
    immer::array<PushCondition> conditions;

/// The glob-style pattern to match against.  Only applicable to ``content``
/// rules.
    std::string pattern;
};

}
namespace nlohmann
{
using namespace Kazv;
template<>
struct adl_serializer<PushRule> {
  static void to_json(json& jo, const PushRule &pod)
  {
  
  
    jo["actions"s] = pod.actions;
    
    jo["default"s] = pod.isDefault;
    
    jo["enabled"s] = pod.enabled;
    
    jo["rule_id"s] = pod.ruleId;
    
    
    addToJsonIfNeeded(jo, "conditions"s, pod.conditions);
    
    addToJsonIfNeeded(jo, "pattern"s, pod.pattern);
  }
  static void from_json(const json &jo, PushRule& result)
  {
  
    result.actions = jo.at("actions"s);
    result.isDefault = jo.at("default"s);
    result.enabled = jo.at("enabled"s);
    result.ruleId = jo.at("rule_id"s);
    result.conditions = jo.at("conditions"s);
    result.pattern = jo.at("pattern"s);
  
  }
};
    }

    namespace Kazv
    {
} // namespace Kazv
