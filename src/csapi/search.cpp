/******************************************************************************
 * THIS FILE IS GENERATED - ANY EDITS WILL BE OVERWRITTEN
 */

#include <algorithm>

#include "search.hpp"

namespace Kazv
{


BaseJob::Query SearchJob::buildQuery(
std::string nextBatch)
{
BaseJob::Query _q;
  
    addToQueryIfNeeded(_q, "next_batch"s, nextBatch);
return _q;
}

    BaseJob::Body SearchJob::buildBody(Categories searchCategories, std::string nextBatch)
      {
      // ignore unused param
      (void)(searchCategories);(void)(nextBatch);
      
        json _data
        ;
        
            _data["search_categories"s] = searchCategories;
          
        return BaseJob::JsonBody(_data);
        

      };

      

SearchJob::SearchJob(
        std::string serverUrl
        , std::string _accessToken
        ,
        Categories searchCategories, std::string nextBatch)
      : BaseJob(std::move(serverUrl),
          std::string("/_matrix/client/r0") + "/search",
          POST,
          _accessToken,
          ReturnType::Json,
            buildBody(searchCategories, nextBatch)
              , buildQuery(nextBatch)
                )
        {
        }

          bool SearchJob::success(Response r)
          {
            return BaseJob::success(r)
            
              && isBodyJson(r.body)
            && jsonBody(r).get().contains("search_categories"s)
          ;
          }


    
    SearchJob::ResultCategories SearchJob::searchCategories(Response r)
    {
    if (jsonBody(r).get()
    .contains("search_categories"s)) {
    return
    jsonBody(r).get()["search_categories"s]
    /*.get<ResultCategories>()*/;}
    else { return ResultCategories(  );}
    }

}
