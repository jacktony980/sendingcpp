/******************************************************************************
 * THIS FILE IS GENERATED - ANY EDITS WILL BE OVERWRITTEN
 */

#include "users.hpp"

namespace Kazv
{
  

    BaseJob::Body SearchUserDirectoryJob::buildBody(std::string searchTerm, std::optional<int> limit)
      {
      // ignore unused param
      (void)(searchTerm);(void)(limit);
      
        json _data
        ;
        
            _data["search_term"s] = searchTerm;
          
          
            addToJsonIfNeeded(_data, "limit"s, limit);
        return BaseJob::JsonBody(_data);
        

      };

SearchUserDirectoryJob::SearchUserDirectoryJob(
        std::string serverUrl
        , std::string _accessToken
        ,
        std::string searchTerm, std::optional<int> limit)
      : BaseJob(std::move(serverUrl),
          std::string("/_matrix/client/r0") + "/user_directory/search",
          POST,
          _accessToken,
          ReturnType::Json,
            buildBody(searchTerm, limit)
      )
        {
        
        
          //addExpectedKey("results");
          //addExpectedKey("limited");
        }


    
    immer::array<SearchUserDirectoryJob::User> SearchUserDirectoryJob::results(Response r)
    {
    if (jsonBody(r).get()
    .contains("results"s)) {
    return
    jsonBody(r).get()["results"s]
    /*.get<immer::array<User>>()*/;}
    else { return immer::array<User>(  );}
    }

    
    bool SearchUserDirectoryJob::limited(Response r)
    {
    if (jsonBody(r).get()
    .contains("limited"s)) {
    return
    jsonBody(r).get()["limited"s]
    /*.get<bool>()*/;}
    else { return bool(  );}
    }

}
