/******************************************************************************
 * THIS FILE IS GENERATED - ANY EDITS WILL BE OVERWRITTEN
 */

#pragma once

#include "basejob.hpp"


namespace Kazv {

/*! \brief List the tags for a room.
 *
 * List the tags set by a user on a room.
 */
class GetRoomTagsJob : public BaseJob {
public:
  // Inner data structures

/// List the tags set by a user on a room.
    struct Tag
        {
/// A number in a range ``[0,1]`` describing a relative
/// position of the room under the given tag.
          std::optional<float> order;
/// List the tags set by a user on a room.
          JsonWrap additionalProperties;
        };


          static constexpr auto needsAuth() {
          return true
            ;
              }


// Construction/destruction

  /*! \brief List the tags for a room.
 *
    * \param userId
    *   The id of the user to get tags for. The access token must be
    *   authorized to make requests for this user ID.
    * 
    * \param roomId
    *   The ID of the room to get tags for.
    */
    explicit GetRoomTagsJob(std::string serverUrl
    , std::string _accessToken
      ,
        std::string userId , std::string roomId );


    // Result properties
        
        

    
/// List the tags set by a user on a room.
static immer::map<std::string, Tag> tags(Response r);

    static BaseJob::Query buildQuery(
    );

      static BaseJob::Body buildBody(std::string userId, std::string roomId);

        static bool success(Response r);
        
      };

      } 
      namespace nlohmann
      {
      using namespace Kazv;
      template<>
      struct adl_serializer<GetRoomTagsJob::Tag> {

  static void from_json(const json &jo, GetRoomTagsJob::Tag& result)
  {
  
    if (jo.contains("order"s)) {
      result.order = jo.at("order"s);
    }
    result.additionalProperties = jo;
  }
};
    }

    namespace Kazv
    {

/*! \brief Add a tag to a room.
 *
 * Add a tag to the room.
 */
class SetRoomTagJob : public BaseJob {
public:


          static constexpr auto needsAuth() {
          return true
            ;
              }


// Construction/destruction

  /*! \brief Add a tag to a room.
 *
    * \param userId
    *   The id of the user to add a tag for. The access token must be
    *   authorized to make requests for this user ID.
    * 
    * \param roomId
    *   The ID of the room to add a tag to.
    * 
    * \param tag
    *   The tag to add.
    * 
    * \param order
    *   A number in a range ``[0,1]`` describing a relative
    *   position of the room under the given tag.
    * 
    * \param additionalProperties
    *   Add a tag to the room.
    */
    explicit SetRoomTagJob(std::string serverUrl
    , std::string _accessToken
      ,
        std::string userId , std::string roomId , std::string tag , std::optional<float> order  = std::nullopt, JsonWrap additionalProperties  = {});
    

    static BaseJob::Query buildQuery(
    );

      static BaseJob::Body buildBody(std::string userId, std::string roomId, std::string tag, std::optional<float> order, JsonWrap additionalProperties);

        static bool success(Response r);
        
      };

      } 
      namespace nlohmann
      {
      using namespace Kazv;
    
    }

    namespace Kazv
    {

/*! \brief Remove a tag from the room.
 *
 * Remove a tag from the room.
 */
class DeleteRoomTagJob : public BaseJob {
public:


          static constexpr auto needsAuth() {
          return true
            ;
              }


// Construction/destruction

  /*! \brief Remove a tag from the room.
 *
    * \param userId
    *   The id of the user to remove a tag for. The access token must be
    *   authorized to make requests for this user ID.
    * 
    * \param roomId
    *   The ID of the room to remove a tag from.
    * 
    * \param tag
    *   The tag to remove.
    */
    explicit DeleteRoomTagJob(std::string serverUrl
    , std::string _accessToken
      ,
        std::string userId , std::string roomId , std::string tag );


    static BaseJob::Query buildQuery(
    );

      static BaseJob::Body buildBody(std::string userId, std::string roomId, std::string tag);

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
