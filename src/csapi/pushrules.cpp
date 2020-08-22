/******************************************************************************
 * THIS FILE IS GENERATED - ANY EDITS WILL BE OVERWRITTEN
 */

#include "pushrules.hpp"

namespace Kazv
{
  

    BaseJob::Body GetPushRulesJob::buildBody()
      {
      // ignore unused param
      
      
      
              return BaseJob::EmptyBody{};

      };

GetPushRulesJob::GetPushRulesJob(
        std::string serverUrl
        , std::string _accessToken
        
        )
      : BaseJob(std::move(serverUrl),
          std::string("/_matrix/client/r0") + "/pushrules",
          GET,
          _accessToken,
          ReturnType::Json,
            buildBody()
      )
        {
        
        
          //addExpectedKey("global");
        }


    
    PushRuleset GetPushRulesJob::global(Response r)
    {
    if (jsonBody(r).get()
    .contains("global"s)) {
    return
    jsonBody(r).get()["global"s]
    /*.get<PushRuleset>()*/;}
    else { return PushRuleset(  );}
    }

  

    BaseJob::Body GetPushRuleJob::buildBody(std::string scope, std::string kind, std::string ruleId)
      {
      // ignore unused param
      (void)(scope);(void)(kind);(void)(ruleId);
      
      
              return BaseJob::EmptyBody{};

      };

GetPushRuleJob::GetPushRuleJob(
        std::string serverUrl
        , std::string _accessToken
        ,
        std::string scope, std::string kind, std::string ruleId)
      : BaseJob(std::move(serverUrl),
          std::string("/_matrix/client/r0") + "/pushrules/" + scope + "/" + kind + "/" + ruleId,
          GET,
          _accessToken,
          ReturnType::Json,
            buildBody(scope, kind, ruleId)
      )
        {
        
        
        }


  

    BaseJob::Body DeletePushRuleJob::buildBody(std::string scope, std::string kind, std::string ruleId)
      {
      // ignore unused param
      (void)(scope);(void)(kind);(void)(ruleId);
      
      
              return BaseJob::EmptyBody{};

      };

DeletePushRuleJob::DeletePushRuleJob(
        std::string serverUrl
        , std::string _accessToken
        ,
        std::string scope, std::string kind, std::string ruleId)
      : BaseJob(std::move(serverUrl),
          std::string("/_matrix/client/r0") + "/pushrules/" + scope + "/" + kind + "/" + ruleId,
          DELETE,
          _accessToken,
          ReturnType::Json,
            buildBody(scope, kind, ruleId)
      )
        {
        
        
        }



BaseJob::Query SetPushRuleJob::buildQuery(
std::string before, std::string after)
{
BaseJob::Query _q;
  
    addToQueryIfNeeded(_q, "before"s, before);
  
    addToQueryIfNeeded(_q, "after"s, after);
return _q;
}

    BaseJob::Body SetPushRuleJob::buildBody(std::string scope, std::string kind, std::string ruleId, immer::array<Variant> actions, std::string before, std::string after, immer::array<PushCondition> conditions, std::string pattern)
      {
      // ignore unused param
      (void)(scope);(void)(kind);(void)(ruleId);(void)(actions);(void)(before);(void)(after);(void)(conditions);(void)(pattern);
      
        json _data
        ;
        
            _data["actions"s] = actions;
          
          
            addToJsonIfNeeded(_data, "conditions"s, conditions);
          
            addToJsonIfNeeded(_data, "pattern"s, pattern);
        return BaseJob::JsonBody(_data);
        

      };

SetPushRuleJob::SetPushRuleJob(
        std::string serverUrl
        , std::string _accessToken
        ,
        std::string scope, std::string kind, std::string ruleId, immer::array<Variant> actions, std::string before, std::string after, immer::array<PushCondition> conditions, std::string pattern)
      : BaseJob(std::move(serverUrl),
          std::string("/_matrix/client/r0") + "/pushrules/" + scope + "/" + kind + "/" + ruleId,
          PUT,
          _accessToken,
          ReturnType::Json,
            buildBody(scope, kind, ruleId, actions, before, after, conditions, pattern)
      , buildQuery(before, after))
        {
        
        
        }


  

    BaseJob::Body IsPushRuleEnabledJob::buildBody(std::string scope, std::string kind, std::string ruleId)
      {
      // ignore unused param
      (void)(scope);(void)(kind);(void)(ruleId);
      
      
              return BaseJob::EmptyBody{};

      };

IsPushRuleEnabledJob::IsPushRuleEnabledJob(
        std::string serverUrl
        , std::string _accessToken
        ,
        std::string scope, std::string kind, std::string ruleId)
      : BaseJob(std::move(serverUrl),
          std::string("/_matrix/client/r0") + "/pushrules/" + scope + "/" + kind + "/" + ruleId + "/enabled",
          GET,
          _accessToken,
          ReturnType::Json,
            buildBody(scope, kind, ruleId)
      )
        {
        
        
          //addExpectedKey("enabled");
        }


    
    bool IsPushRuleEnabledJob::enabled(Response r)
    {
    if (jsonBody(r).get()
    .contains("enabled"s)) {
    return
    jsonBody(r).get()["enabled"s]
    /*.get<bool>()*/;}
    else { return bool(  );}
    }

  

    BaseJob::Body SetPushRuleEnabledJob::buildBody(std::string scope, std::string kind, std::string ruleId, bool enabled)
      {
      // ignore unused param
      (void)(scope);(void)(kind);(void)(ruleId);(void)(enabled);
      
        json _data
        ;
        
            _data["enabled"s] = enabled;
          
        return BaseJob::JsonBody(_data);
        

      };

SetPushRuleEnabledJob::SetPushRuleEnabledJob(
        std::string serverUrl
        , std::string _accessToken
        ,
        std::string scope, std::string kind, std::string ruleId, bool enabled)
      : BaseJob(std::move(serverUrl),
          std::string("/_matrix/client/r0") + "/pushrules/" + scope + "/" + kind + "/" + ruleId + "/enabled",
          PUT,
          _accessToken,
          ReturnType::Json,
            buildBody(scope, kind, ruleId, enabled)
      )
        {
        
        
        }


  

    BaseJob::Body GetPushRuleActionsJob::buildBody(std::string scope, std::string kind, std::string ruleId)
      {
      // ignore unused param
      (void)(scope);(void)(kind);(void)(ruleId);
      
      
              return BaseJob::EmptyBody{};

      };

GetPushRuleActionsJob::GetPushRuleActionsJob(
        std::string serverUrl
        , std::string _accessToken
        ,
        std::string scope, std::string kind, std::string ruleId)
      : BaseJob(std::move(serverUrl),
          std::string("/_matrix/client/r0") + "/pushrules/" + scope + "/" + kind + "/" + ruleId + "/actions",
          GET,
          _accessToken,
          ReturnType::Json,
            buildBody(scope, kind, ruleId)
      )
        {
        
        
          //addExpectedKey("actions");
        }


    
    immer::array<Variant> GetPushRuleActionsJob::actions(Response r)
    {
    if (jsonBody(r).get()
    .contains("actions"s)) {
    return
    jsonBody(r).get()["actions"s]
    /*.get<immer::array<Variant>>()*/;}
    else { return immer::array<Variant>(  );}
    }

  

    BaseJob::Body SetPushRuleActionsJob::buildBody(std::string scope, std::string kind, std::string ruleId, immer::array<Variant> actions)
      {
      // ignore unused param
      (void)(scope);(void)(kind);(void)(ruleId);(void)(actions);
      
        json _data
        ;
        
            _data["actions"s] = actions;
          
        return BaseJob::JsonBody(_data);
        

      };

SetPushRuleActionsJob::SetPushRuleActionsJob(
        std::string serverUrl
        , std::string _accessToken
        ,
        std::string scope, std::string kind, std::string ruleId, immer::array<Variant> actions)
      : BaseJob(std::move(serverUrl),
          std::string("/_matrix/client/r0") + "/pushrules/" + scope + "/" + kind + "/" + ruleId + "/actions",
          PUT,
          _accessToken,
          ReturnType::Json,
            buildBody(scope, kind, ruleId, actions)
      )
        {
        
        
        }


}
