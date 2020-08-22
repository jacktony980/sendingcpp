/******************************************************************************
 * THIS FILE IS GENERATED - ANY EDITS WILL BE OVERWRITTEN
 */

#include "content-repo.hpp"

namespace Kazv
{

BaseJob::Query UploadContentJob::buildQuery(
std::string filename)
{
BaseJob::Query _q;
  
    addToQueryIfNeeded(_q, "filename"s, filename);
return _q;
}

    BaseJob::Body UploadContentJob::buildBody(Bytes content, std::string filename, std::string contentType)
      {
      // ignore unused param
      (void)(content);(void)(filename);(void)(contentType);
        return BaseJob::BytesBody(content)
          ;
      
          

      };

UploadContentJob::UploadContentJob(
        std::string serverUrl
        , std::string _accessToken
        ,
        Bytes content, std::string filename, std::string contentType)
      : BaseJob(std::move(serverUrl),
          std::string("/_matrix/media/r0") + "/upload",
          POST,
          _accessToken,
          ReturnType::Json,
            buildBody(content, filename, contentType)
      , buildQuery(filename))
        {
          //setRequestHeader("Content-Type", contentType.toLatin1());
        
          //addExpectedKey("content_uri");
        }


    
    std::string UploadContentJob::contentUri(Response r)
    {
    if (jsonBody(r).get()
    .contains("content_uri"s)) {
    return
    jsonBody(r).get()["content_uri"s]
    /*.get<std::string>()*/;}
    else { return std::string(  );}
    }


BaseJob::Query GetContentJob::buildQuery(
bool allowRemote)
{
BaseJob::Query _q;
  
    addToQueryIfNeeded(_q, "allow_remote"s, allowRemote);
return _q;
}

    BaseJob::Body GetContentJob::buildBody(std::string serverName, std::string mediaId, bool allowRemote)
      {
      // ignore unused param
      (void)(serverName);(void)(mediaId);(void)(allowRemote);
      
      
              return BaseJob::EmptyBody{};

      };

GetContentJob::GetContentJob(
        std::string serverUrl
        
        ,
        std::string serverName, std::string mediaId, bool allowRemote)
      : BaseJob(std::move(serverUrl),
          std::string("/_matrix/media/r0") + "/download/" + serverName + "/" + mediaId,
          GET,
           {} ,
          ReturnType::Byte,
            buildBody(serverName, mediaId, allowRemote)
      , buildQuery(allowRemote))
        {
        
          //setExpectedContentTypes({ "*/*" });
        
        }



BaseJob::Query GetContentOverrideNameJob::buildQuery(
bool allowRemote)
{
BaseJob::Query _q;
  
    addToQueryIfNeeded(_q, "allow_remote"s, allowRemote);
return _q;
}

    BaseJob::Body GetContentOverrideNameJob::buildBody(std::string serverName, std::string mediaId, std::string fileName, bool allowRemote)
      {
      // ignore unused param
      (void)(serverName);(void)(mediaId);(void)(fileName);(void)(allowRemote);
      
      
              return BaseJob::EmptyBody{};

      };

GetContentOverrideNameJob::GetContentOverrideNameJob(
        std::string serverUrl
        
        ,
        std::string serverName, std::string mediaId, std::string fileName, bool allowRemote)
      : BaseJob(std::move(serverUrl),
          std::string("/_matrix/media/r0") + "/download/" + serverName + "/" + mediaId + "/" + fileName,
          GET,
           {} ,
          ReturnType::Byte,
            buildBody(serverName, mediaId, fileName, allowRemote)
      , buildQuery(allowRemote))
        {
        
          //setExpectedContentTypes({ "*/*" });
        
        }



BaseJob::Query GetContentThumbnailJob::buildQuery(
int width, int height, std::string method, bool allowRemote)
{
BaseJob::Query _q;
    addToQuery(_q, "width"s, width);
  
    addToQuery(_q, "height"s, height);
  
  
    addToQueryIfNeeded(_q, "method"s, method);
  
    addToQueryIfNeeded(_q, "allow_remote"s, allowRemote);
return _q;
}

    BaseJob::Body GetContentThumbnailJob::buildBody(std::string serverName, std::string mediaId, int width, int height, std::string method, bool allowRemote)
      {
      // ignore unused param
      (void)(serverName);(void)(mediaId);(void)(width);(void)(height);(void)(method);(void)(allowRemote);
      
      
              return BaseJob::EmptyBody{};

      };

GetContentThumbnailJob::GetContentThumbnailJob(
        std::string serverUrl
        
        ,
        std::string serverName, std::string mediaId, int width, int height, std::string method, bool allowRemote)
      : BaseJob(std::move(serverUrl),
          std::string("/_matrix/media/r0") + "/thumbnail/" + serverName + "/" + mediaId,
          GET,
           {} ,
          ReturnType::Byte,
            buildBody(serverName, mediaId, width, height, method, allowRemote)
      , buildQuery(width, height, method, allowRemote))
        {
        
          //setExpectedContentTypes({ "image/jpeg", "image/png" });
        
        }



BaseJob::Query GetUrlPreviewJob::buildQuery(
std::string url, std::optional<std::int_fast64_t> ts)
{
BaseJob::Query _q;
    addToQuery(_q, "url"s, url);
  
  
    addToQueryIfNeeded(_q, "ts"s, ts);
return _q;
}

    BaseJob::Body GetUrlPreviewJob::buildBody(std::string url, std::optional<std::int_fast64_t> ts)
      {
      // ignore unused param
      (void)(url);(void)(ts);
      
      
              return BaseJob::EmptyBody{};

      };

GetUrlPreviewJob::GetUrlPreviewJob(
        std::string serverUrl
        , std::string _accessToken
        ,
        std::string url, std::optional<std::int_fast64_t> ts)
      : BaseJob(std::move(serverUrl),
          std::string("/_matrix/media/r0") + "/preview_url",
          GET,
          _accessToken,
          ReturnType::Json,
            buildBody(url, ts)
      , buildQuery(url, ts))
        {
        
        
        }


    
    std::optional<std::int_fast64_t> GetUrlPreviewJob::matrixImageSize(Response r)
    {
    if (jsonBody(r).get()
    .contains("matrix:image:size"s)) {
    return
    jsonBody(r).get()["matrix:image:size"s]
    /*.get<std::int_fast64_t>()*/;}
    else { return std::optional<std::int_fast64_t>(  );}
    }

    
    std::string GetUrlPreviewJob::ogImage(Response r)
    {
    if (jsonBody(r).get()
    .contains("og:image"s)) {
    return
    jsonBody(r).get()["og:image"s]
    /*.get<std::string>()*/;}
    else { return std::string(  );}
    }

  

    BaseJob::Body GetConfigJob::buildBody()
      {
      // ignore unused param
      
      
      
              return BaseJob::EmptyBody{};

      };

GetConfigJob::GetConfigJob(
        std::string serverUrl
        , std::string _accessToken
        
        )
      : BaseJob(std::move(serverUrl),
          std::string("/_matrix/media/r0") + "/config",
          GET,
          _accessToken,
          ReturnType::Json,
            buildBody()
      )
        {
        
        
        }


    
    std::optional<std::int_fast64_t> GetConfigJob::uploadSize(Response r)
    {
    if (jsonBody(r).get()
    .contains("m.upload.size"s)) {
    return
    jsonBody(r).get()["m.upload.size"s]
    /*.get<std::int_fast64_t>()*/;}
    else { return std::optional<std::int_fast64_t>(  );}
    }

}
