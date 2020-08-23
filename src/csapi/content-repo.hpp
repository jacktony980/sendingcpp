/******************************************************************************
 * THIS FILE IS GENERATED - ANY EDITS WILL BE OVERWRITTEN
 */

#pragma once

#include "basejob.hpp"
#include "types.hpp"

namespace Kazv {

/*! \brief Upload some content to the content repository.
 *
 */
class UploadContentJob : public BaseJob {
public:


// Construction/destruction

  /*! \brief Upload some content to the content repository.
 *
    * \param content
    *   The content to be uploaded.
    * 
    * \param filename
    *   The name of the file being uploaded
    * 
    * \param contentType
    *   The content type of the file being uploaded
    */
    explicit UploadContentJob(std::string serverUrl
    , std::string _accessToken
      ,
        Bytes content , std::string filename  = {}, std::string contentType  = {});
    

    // Result properties
        
        

    
/// The `MXC URI`_ to the uploaded content.
static std::string contentUri(Response r);

    static BaseJob::Query buildQuery(
    std::string filename);

      static BaseJob::Body buildBody(Bytes content, std::string filename, std::string contentType);

        static bool success(Response r);
        
      };

      } 
      namespace nlohmann
      {
      using namespace Kazv;
    
    }

    namespace Kazv
    {

/*! \brief Download content from the content repository.
 *
 */
class GetContentJob : public BaseJob {
public:


// Construction/destruction

  /*! \brief Download content from the content repository.
 *
    * \param serverName
    *   The server name from the ``mxc://`` URI (the authoritory component)
    * 
    * \param mediaId
    *   The media ID from the ``mxc://`` URI (the path component)
    * 
    * \param allowRemote
    *   Indicates to the server that it should not attempt to fetch the media if it is deemed
    *   remote. This is to prevent routing loops where the server contacts itself. Defaults to
    *   true if not provided.
    */
    explicit GetContentJob(std::string serverUrl
    
      ,
        std::string serverName , std::string mediaId , bool allowRemote  = true);


    // Result properties
/*
/// The content type of the file that was previously uploaded.
static std::string contentType(Response r)
    {
        return reply()->rawHeader("Content-Type");
    }
*/        
/*
/// The name of the file that was previously uploaded, if set.
static std::string contentDisposition(Response r)
    {
        return reply()->rawHeader("Content-Disposition");
    }
*/        

/// The content that was previously uploaded.
    static Bytes data(Response r)
    {
    return
      std::get<Bytes>(r.body)
    
    ;
    }
        

    static BaseJob::Query buildQuery(
    bool allowRemote);

      static BaseJob::Body buildBody(std::string serverName, std::string mediaId, bool allowRemote);

        static bool success(Response r);
          static const immer::array<std::string> expectedContentTypes;
      };

      } 
      namespace nlohmann
      {
      using namespace Kazv;
    
    }

    namespace Kazv
    {

/*! \brief Download content from the content repository overriding the file name
 *
 * This will download content from the content repository (same as
 * the previous endpoint) but replace the target file name with the one
 * provided by the caller.
 */
class GetContentOverrideNameJob : public BaseJob {
public:


// Construction/destruction

  /*! \brief Download content from the content repository overriding the file name
 *
    * \param serverName
    *   The server name from the ``mxc://`` URI (the authoritory component)
    * 
    * \param mediaId
    *   The media ID from the ``mxc://`` URI (the path component)
    * 
    * \param fileName
    *   A filename to give in the ``Content-Disposition`` header.
    * 
    * \param allowRemote
    *   Indicates to the server that it should not attempt to fetch the media if it is deemed
    *   remote. This is to prevent routing loops where the server contacts itself. Defaults to
    *   true if not provided.
    */
    explicit GetContentOverrideNameJob(std::string serverUrl
    
      ,
        std::string serverName , std::string mediaId , std::string fileName , bool allowRemote  = true);


    // Result properties
/*
/// The content type of the file that was previously uploaded.
static std::string contentType(Response r)
    {
        return reply()->rawHeader("Content-Type");
    }
*/        
/*
/// The ``fileName`` requested or the name of the file that was previously
/// uploaded, if set.
static std::string contentDisposition(Response r)
    {
        return reply()->rawHeader("Content-Disposition");
    }
*/        

/// The content that was previously uploaded.
    static Bytes data(Response r)
    {
    return
      std::get<Bytes>(r.body)
    
    ;
    }
        

    static BaseJob::Query buildQuery(
    bool allowRemote);

      static BaseJob::Body buildBody(std::string serverName, std::string mediaId, std::string fileName, bool allowRemote);

        static bool success(Response r);
          static const immer::array<std::string> expectedContentTypes;
      };

      } 
      namespace nlohmann
      {
      using namespace Kazv;
    
    }

    namespace Kazv
    {

/*! \brief Download a thumbnail of content from the content repository
 *
 * Download a thumbnail of content from the content repository.
 * See the `thumbnailing <#thumbnails>`_ section for more information.
 */
class GetContentThumbnailJob : public BaseJob {
public:


// Construction/destruction

  /*! \brief Download a thumbnail of content from the content repository
 *
    * \param serverName
    *   The server name from the ``mxc://`` URI (the authoritory component)
    * 
    * \param mediaId
    *   The media ID from the ``mxc://`` URI (the path component)
    * 
    * \param width
    *   The *desired* width of the thumbnail. The actual thumbnail may be
    *   larger than the size specified.
    * 
    * \param height
    *   The *desired* height of the thumbnail. The actual thumbnail may be
    *   larger than the size specified.
    * 
    * \param method
    *   The desired resizing method. See the `thumbnailing <#thumbnails>`_
    *   section for more information.
    * 
    * \param allowRemote
    *   Indicates to the server that it should not attempt to fetch
    *   the media if it is deemed remote. This is to prevent routing loops
    *   where the server contacts itself. Defaults to true if not provided.
    */
    explicit GetContentThumbnailJob(std::string serverUrl
    
      ,
        std::string serverName , std::string mediaId , int width , int height , std::string method  = {}, bool allowRemote  = true);


    // Result properties
/*
/// The content type of the thumbnail.
static std::string contentType(Response r)
    {
        return reply()->rawHeader("Content-Type");
    }
*/        

/// A thumbnail of the requested content.
    static Bytes data(Response r)
    {
    return
      std::get<Bytes>(r.body)
    
    ;
    }
        

    static BaseJob::Query buildQuery(
    int width, int height, std::string method, bool allowRemote);

      static BaseJob::Body buildBody(std::string serverName, std::string mediaId, int width, int height, std::string method, bool allowRemote);

        static bool success(Response r);
          static const immer::array<std::string> expectedContentTypes;
      };

      } 
      namespace nlohmann
      {
      using namespace Kazv;
    
    }

    namespace Kazv
    {

/*! \brief Get information about a URL for a client
 *
 * Get information about a URL for the client. Typically this is called when a
 * client sees a URL in a message and wants to render a preview for the user.
 * 
 * .. Note::
 *   Clients should consider avoiding this endpoint for URLs posted in encrypted
 *   rooms. Encrypted rooms often contain more sensitive information the users
 *   do not want to share with the homeserver, and this can mean that the URLs
 *   being shared should also not be shared with the homeserver.
 */
class GetUrlPreviewJob : public BaseJob {
public:


// Construction/destruction

  /*! \brief Get information about a URL for a client
 *
    * \param url
    *   The URL to get a preview of.
    * 
    * \param ts
    *   The preferred point in time to return a preview for. The server may
    *   return a newer version if it does not have the requested version
    *   available.
    */
    explicit GetUrlPreviewJob(std::string serverUrl
    , std::string _accessToken
      ,
        std::string url , std::optional<std::int_fast64_t> ts  = std::nullopt);


    // Result properties
        
        

    
/// The byte-size of the image. Omitted if there is no image attached.
static std::optional<std::int_fast64_t> matrixImageSize(Response r);

    
/// An `MXC URI`_ to the image. Omitted if there is no image.
static std::string ogImage(Response r);

    static BaseJob::Query buildQuery(
    std::string url, std::optional<std::int_fast64_t> ts);

      static BaseJob::Body buildBody(std::string url, std::optional<std::int_fast64_t> ts);

        static bool success(Response r);
        
      };

      } 
      namespace nlohmann
      {
      using namespace Kazv;
    
    }

    namespace Kazv
    {

/*! \brief Get the configuration for the content repository.
 *
 * This endpoint allows clients to retrieve the configuration of the content
 * repository, such as upload limitations.
 * Clients SHOULD use this as a guide when using content repository endpoints.
 * All values are intentionally left optional. Clients SHOULD follow
 * the advice given in the field description when the field is not available.
 * 
 * **NOTE:** Both clients and server administrators should be aware that proxies
 * between the client and the server may affect the apparent behaviour of content
 * repository APIs, for example, proxies may enforce a lower upload size limit
 * than is advertised by the server on this endpoint.
 */
class GetConfigJob : public BaseJob {
public:


// Construction/destruction

    /// Get the configuration for the content repository.
    explicit GetConfigJob(std::string serverUrl
    , std::string _accessToken
      
        );


    // Result properties
        
        

    
/// The maximum size an upload can be in bytes.
/// Clients SHOULD use this as a guide when uploading content.
/// If not listed or null, the size limit should be treated as unknown.
static std::optional<std::int_fast64_t> uploadSize(Response r);

    static BaseJob::Query buildQuery(
    );

      static BaseJob::Body buildBody();

        static bool success(Response r);
        
      };

      } 
      namespace nlohmann
      {
      using namespace Kazv;
    
    }

    namespace Kazv
    {

} // namespace Kazv
