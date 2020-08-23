/******************************************************************************
 * THIS FILE IS GENERATED - ANY EDITS WILL BE OVERWRITTEN
 */

#include <algorithm>

#include "third_party_lookup.hpp"

namespace Kazv
{


BaseJob::Query GetProtocolsJob::buildQuery(
)
{
BaseJob::Query _q;

return _q;
}

    BaseJob::Body GetProtocolsJob::buildBody()
      {
      // ignore unused param
      
      
      
              return BaseJob::EmptyBody{};

      };

      

GetProtocolsJob::GetProtocolsJob(
        std::string serverUrl
        , std::string _accessToken
        
        )
      : BaseJob(std::move(serverUrl),
          std::string("/_matrix/client/r0") + "/thirdparty/protocols",
          GET,
          _accessToken,
          ReturnType::Json,
            buildBody()
              , buildQuery()
                )
        {
        }

          bool GetProtocolsJob::success(Response r)
          {
            return BaseJob::success(r)
            
              && isBodyJson(r.body)
          ;
          }




BaseJob::Query GetProtocolMetadataJob::buildQuery(
)
{
BaseJob::Query _q;

return _q;
}

    BaseJob::Body GetProtocolMetadataJob::buildBody(std::string protocol)
      {
      // ignore unused param
      (void)(protocol);
      
      
              return BaseJob::EmptyBody{};

      };

      

GetProtocolMetadataJob::GetProtocolMetadataJob(
        std::string serverUrl
        , std::string _accessToken
        ,
        std::string protocol)
      : BaseJob(std::move(serverUrl),
          std::string("/_matrix/client/r0") + "/thirdparty/protocol/" + protocol,
          GET,
          _accessToken,
          ReturnType::Json,
            buildBody(protocol)
              , buildQuery()
                )
        {
        }

          bool GetProtocolMetadataJob::success(Response r)
          {
            return BaseJob::success(r)
            
              && isBodyJson(r.body)
          ;
          }




BaseJob::Query QueryLocationByProtocolJob::buildQuery(
std::string searchFields)
{
BaseJob::Query _q;
  
    addToQueryIfNeeded(_q, "searchFields"s, searchFields);
return _q;
}

    BaseJob::Body QueryLocationByProtocolJob::buildBody(std::string protocol, std::string searchFields)
      {
      // ignore unused param
      (void)(protocol);(void)(searchFields);
      
      
              return BaseJob::EmptyBody{};

      };

      

QueryLocationByProtocolJob::QueryLocationByProtocolJob(
        std::string serverUrl
        , std::string _accessToken
        ,
        std::string protocol, std::string searchFields)
      : BaseJob(std::move(serverUrl),
          std::string("/_matrix/client/r0") + "/thirdparty/location/" + protocol,
          GET,
          _accessToken,
          ReturnType::Json,
            buildBody(protocol, searchFields)
              , buildQuery(searchFields)
                )
        {
        }

          bool QueryLocationByProtocolJob::success(Response r)
          {
            return BaseJob::success(r)
            
              && isBodyJson(r.body)
          ;
          }




BaseJob::Query QueryUserByProtocolJob::buildQuery(
std::string fields)
{
BaseJob::Query _q;
  
    addToQueryIfNeeded(_q, "fields..."s, fields);
return _q;
}

    BaseJob::Body QueryUserByProtocolJob::buildBody(std::string protocol, std::string fields)
      {
      // ignore unused param
      (void)(protocol);(void)(fields);
      
      
              return BaseJob::EmptyBody{};

      };

      

QueryUserByProtocolJob::QueryUserByProtocolJob(
        std::string serverUrl
        , std::string _accessToken
        ,
        std::string protocol, std::string fields)
      : BaseJob(std::move(serverUrl),
          std::string("/_matrix/client/r0") + "/thirdparty/user/" + protocol,
          GET,
          _accessToken,
          ReturnType::Json,
            buildBody(protocol, fields)
              , buildQuery(fields)
                )
        {
        }

          bool QueryUserByProtocolJob::success(Response r)
          {
            return BaseJob::success(r)
            
              && isBodyJson(r.body)
          ;
          }




BaseJob::Query QueryLocationByAliasJob::buildQuery(
std::string alias)
{
BaseJob::Query _q;
    addToQuery(_q, "alias"s, alias);
  
return _q;
}

    BaseJob::Body QueryLocationByAliasJob::buildBody(std::string alias)
      {
      // ignore unused param
      (void)(alias);
      
      
              return BaseJob::EmptyBody{};

      };

      

QueryLocationByAliasJob::QueryLocationByAliasJob(
        std::string serverUrl
        , std::string _accessToken
        ,
        std::string alias)
      : BaseJob(std::move(serverUrl),
          std::string("/_matrix/client/r0") + "/thirdparty/location",
          GET,
          _accessToken,
          ReturnType::Json,
            buildBody(alias)
              , buildQuery(alias)
                )
        {
        }

          bool QueryLocationByAliasJob::success(Response r)
          {
            return BaseJob::success(r)
            
              && isBodyJson(r.body)
          ;
          }




BaseJob::Query QueryUserByIDJob::buildQuery(
std::string userid)
{
BaseJob::Query _q;
    addToQuery(_q, "userid"s, userid);
  
return _q;
}

    BaseJob::Body QueryUserByIDJob::buildBody(std::string userid)
      {
      // ignore unused param
      (void)(userid);
      
      
              return BaseJob::EmptyBody{};

      };

      

QueryUserByIDJob::QueryUserByIDJob(
        std::string serverUrl
        , std::string _accessToken
        ,
        std::string userid)
      : BaseJob(std::move(serverUrl),
          std::string("/_matrix/client/r0") + "/thirdparty/user",
          GET,
          _accessToken,
          ReturnType::Json,
            buildBody(userid)
              , buildQuery(userid)
                )
        {
        }

          bool QueryUserByIDJob::success(Response r)
          {
            return BaseJob::success(r)
            
              && isBodyJson(r.body)
          ;
          }


}
