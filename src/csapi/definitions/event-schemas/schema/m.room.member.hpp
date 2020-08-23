/******************************************************************************
 * THIS FILE IS GENERATED - ANY EDITS WILL BE OVERWRITTEN
 */

#pragma once

#include "types.hpp"
#include "csapi/definitions/event-schemas/schema/stripped_state.hpp"
#include "csapi/definitions/event-schemas/schema/core-event-schema/unsigned_prop.hpp"

namespace Kazv {
/// A block of content which has been signed, which servers can use to verify the event. Clients should ignore this.
struct SignedData
{       

/// The invited matrix user ID. Must be equal to the user_id property of the event.
    std::string mxid;

/// A single signature from the verifying server, in the format specified by the Signing Events section of the server-server API.
    immer::map<std::string, immer::map<std::string, std::string>> signatures;

/// The token property of the containing third_party_invite object.
    std::string token;
};

}
namespace nlohmann
{
using namespace Kazv;
template<>
struct adl_serializer<SignedData> {
  static void to_json(json& jo, const SignedData &pod)
  {
  
  
    jo["mxid"s] = pod.mxid;
    
    jo["signatures"s] = pod.signatures;
    
    jo["token"s] = pod.token;
    
  }
  static void from_json(const json &jo, SignedData& result)
  {
  
    if (jo.contains("mxid"s)) {
      result.mxid = jo.at("mxid"s);
    }
    if (jo.contains("signatures"s)) {
      result.signatures = jo.at("signatures"s);
    }
    if (jo.contains("token"s)) {
      result.token = jo.at("token"s);
    }
  
  }
};
    }

    namespace Kazv
    {

struct Invite
{       

/// A name which can be displayed to represent the user instead of their third party identifier
    std::string displayName;

/// A block of content which has been signed, which servers can use to verify the event. Clients should ignore this.
    SignedData signedData;
};

}
namespace nlohmann
{
using namespace Kazv;
template<>
struct adl_serializer<Invite> {
  static void to_json(json& jo, const Invite &pod)
  {
  
  
    jo["display_name"s] = pod.displayName;
    
    jo["signed"s] = pod.signedData;
    
  }
  static void from_json(const json &jo, Invite& result)
  {
  
    if (jo.contains("display_name"s)) {
      result.displayName = jo.at("display_name"s);
    }
    if (jo.contains("signed"s)) {
      result.signedData = jo.at("signed"s);
    }
  
  }
};
    }

    namespace Kazv
    {

struct EventContent
{       

/// The avatar URL for this user, if any.
    std::string avatarUrl;

/// The display name for this user, if any.
    Variant displayname;

/// The membership state of the user.
    std::string membership;

/// Flag indicating if the room containing this event was created with the intention of being a direct chat. See `Direct Messaging`_.
    std::optional<bool> isDirect;

    
    std::optional<Invite> thirdPartyInvite;
};

}
namespace nlohmann
{
using namespace Kazv;
template<>
struct adl_serializer<EventContent> {
  static void to_json(json& jo, const EventContent &pod)
  {
  
  
    
    addToJsonIfNeeded(jo, "avatar_url"s, pod.avatarUrl);
    
    addToJsonIfNeeded(jo, "displayname"s, pod.displayname);
    jo["membership"s] = pod.membership;
    
    
    addToJsonIfNeeded(jo, "is_direct"s, pod.isDirect);
    
    addToJsonIfNeeded(jo, "third_party_invite"s, pod.thirdPartyInvite);
  }
  static void from_json(const json &jo, EventContent& result)
  {
  
    if (jo.contains("avatar_url"s)) {
      result.avatarUrl = jo.at("avatar_url"s);
    }
    if (jo.contains("displayname"s)) {
      result.displayname = jo.at("displayname"s);
    }
    if (jo.contains("membership"s)) {
      result.membership = jo.at("membership"s);
    }
    if (jo.contains("is_direct"s)) {
      result.isDirect = jo.at("is_direct"s);
    }
    if (jo.contains("third_party_invite"s)) {
      result.thirdPartyInvite = jo.at("third_party_invite"s);
    }
  
  }
};
    }

    namespace Kazv
    {
/// Adjusts the membership state for a user in a room. It is preferable to use the membership APIs (``/rooms/<room id>/invite`` etc) when performing membership actions rather than adjusting the state directly as there are a restricted set of valid transformations. For example, user A cannot force user B to join a room, and trying to force this state change directly will fail.
/// 
/// The following membership states are specified:
/// 
/// - ``invite`` - The user has been invited to join a room, but has not yet joined it. They may not participate in the room until they join.
/// 
/// - ``join`` - The user has joined the room (possibly after accepting an invite), and may participate in it.
/// 
/// - ``leave`` - The user was once joined to the room, but has since left (possibly by choice, or possibly by being kicked).
/// 
/// - ``ban`` - The user has been banned from the room, and is no longer allowed to join it until they are un-banned from the room (by having their membership state set to a value other than ``ban``).
/// 
/// - ``knock`` - This is a reserved word, which currently has no meaning.
/// 
/// The ``third_party_invite`` property will be set if this invite is an ``invite`` event and is the successor of an ``m.room.third_party_invite`` event, and absent otherwise.
/// 
/// This event may also include an ``invite_room_state`` key inside the event's ``unsigned`` data.
/// If present, this contains an array of ``StrippedState`` Events. These events provide information
/// on a subset of state events such as the room name.
/// 
/// The user for which a membership applies is represented by the ``state_key``. Under some conditions,
/// the ``sender`` and ``state_key`` may not match - this may be interpreted as the ``sender`` affecting
/// the membership state of the ``state_key`` user.
/// 
/// The ``membership`` for a given user can change over time. The table below represents the various changes
/// over time and how clients and servers must interpret those changes. Previous membership can be retrieved
/// from the ``prev_content`` object on an event. If not present, the user's previous membership must be assumed
/// as ``leave``.
/// 
/// .. TODO: Improve how this table is written? We use a csv-table to get around vertical header restrictions.
/// 
/// .. csv-table::
///   :header-rows: 1
///   :stub-columns: 1
/// 
///   "","to ``invite``","to ``join``","to ``leave``","to ``ban``","to ``knock``"
///   "from ``invite``","No change.","User joined the room.","If the ``state_key`` is the same as the ``sender``, the user rejected the invite. Otherwise, the ``state_key`` user had their invite revoked.","User was banned.","Not implemented."
///   "from ``join``","Must never happen.","``displayname`` or ``avatar_url`` changed.","If the ``state_key`` is the same as the ``sender``, the user left. Otherwise, the ``state_key`` user was kicked.","User was kicked and banned.","Not implemented."
///   "from ``leave``","New invitation sent.","User joined.","No change.","User was banned.","Not implemented."
///   "from ``ban``","Must never happen.","Must never happen.","User was unbanned.","No change.","Not implemented."
///   "from ``knock``","Not implemented.","Not implemented.","Not implemented.","Not implemented.","Not implemented."
struct TheCurrentMembershipStateOfAUserInTheRoom : JsonWrap
{       

/// Adjusts the membership state for a user in a room. It is preferable to use the membership APIs (``/rooms/<room id>/invite`` etc) when performing membership actions rather than adjusting the state directly as there are a restricted set of valid transformations. For example, user A cannot force user B to join a room, and trying to force this state change directly will fail.
/// 
/// The following membership states are specified:
/// 
/// - ``invite`` - The user has been invited to join a room, but has not yet joined it. They may not participate in the room until they join.
/// 
/// - ``join`` - The user has joined the room (possibly after accepting an invite), and may participate in it.
/// 
/// - ``leave`` - The user was once joined to the room, but has since left (possibly by choice, or possibly by being kicked).
/// 
/// - ``ban`` - The user has been banned from the room, and is no longer allowed to join it until they are un-banned from the room (by having their membership state set to a value other than ``ban``).
/// 
/// - ``knock`` - This is a reserved word, which currently has no meaning.
/// 
/// The ``third_party_invite`` property will be set if this invite is an ``invite`` event and is the successor of an ``m.room.third_party_invite`` event, and absent otherwise.
/// 
/// This event may also include an ``invite_room_state`` key inside the event's ``unsigned`` data.
/// If present, this contains an array of ``StrippedState`` Events. These events provide information
/// on a subset of state events such as the room name.
/// 
/// The user for which a membership applies is represented by the ``state_key``. Under some conditions,
/// the ``sender`` and ``state_key`` may not match - this may be interpreted as the ``sender`` affecting
/// the membership state of the ``state_key`` user.
/// 
/// The ``membership`` for a given user can change over time. The table below represents the various changes
/// over time and how clients and servers must interpret those changes. Previous membership can be retrieved
/// from the ``prev_content`` object on an event. If not present, the user's previous membership must be assumed
/// as ``leave``.
/// 
/// .. TODO: Improve how this table is written? We use a csv-table to get around vertical header restrictions.
/// 
/// .. csv-table::
///   :header-rows: 1
///   :stub-columns: 1
/// 
///   "","to ``invite``","to ``join``","to ``leave``","to ``ban``","to ``knock``"
///   "from ``invite``","No change.","User joined the room.","If the ``state_key`` is the same as the ``sender``, the user rejected the invite. Otherwise, the ``state_key`` user had their invite revoked.","User was banned.","Not implemented."
///   "from ``join``","Must never happen.","``displayname`` or ``avatar_url`` changed.","If the ``state_key`` is the same as the ``sender``, the user left. Otherwise, the ``state_key`` user was kicked.","User was kicked and banned.","Not implemented."
///   "from ``leave``","New invitation sent.","User joined.","No change.","User was banned.","Not implemented."
///   "from ``ban``","Must never happen.","Must never happen.","User was unbanned.","No change.","Not implemented."
///   "from ``knock``","Not implemented.","Not implemented.","Not implemented.","Not implemented.","Not implemented."
    std::optional<EventContent> content;

/// The ``user_id`` this membership event relates to. In all cases except for when ``membership`` is
/// ``join``, the user ID sending the event does not need to match the user ID in the ``state_key``,
/// unlike other events. Regular authorisation rules still apply.
    std::string stateKey;

/// Adjusts the membership state for a user in a room. It is preferable to use the membership APIs (``/rooms/<room id>/invite`` etc) when performing membership actions rather than adjusting the state directly as there are a restricted set of valid transformations. For example, user A cannot force user B to join a room, and trying to force this state change directly will fail.
/// 
/// The following membership states are specified:
/// 
/// - ``invite`` - The user has been invited to join a room, but has not yet joined it. They may not participate in the room until they join.
/// 
/// - ``join`` - The user has joined the room (possibly after accepting an invite), and may participate in it.
/// 
/// - ``leave`` - The user was once joined to the room, but has since left (possibly by choice, or possibly by being kicked).
/// 
/// - ``ban`` - The user has been banned from the room, and is no longer allowed to join it until they are un-banned from the room (by having their membership state set to a value other than ``ban``).
/// 
/// - ``knock`` - This is a reserved word, which currently has no meaning.
/// 
/// The ``third_party_invite`` property will be set if this invite is an ``invite`` event and is the successor of an ``m.room.third_party_invite`` event, and absent otherwise.
/// 
/// This event may also include an ``invite_room_state`` key inside the event's ``unsigned`` data.
/// If present, this contains an array of ``StrippedState`` Events. These events provide information
/// on a subset of state events such as the room name.
/// 
/// The user for which a membership applies is represented by the ``state_key``. Under some conditions,
/// the ``sender`` and ``state_key`` may not match - this may be interpreted as the ``sender`` affecting
/// the membership state of the ``state_key`` user.
/// 
/// The ``membership`` for a given user can change over time. The table below represents the various changes
/// over time and how clients and servers must interpret those changes. Previous membership can be retrieved
/// from the ``prev_content`` object on an event. If not present, the user's previous membership must be assumed
/// as ``leave``.
/// 
/// .. TODO: Improve how this table is written? We use a csv-table to get around vertical header restrictions.
/// 
/// .. csv-table::
///   :header-rows: 1
///   :stub-columns: 1
/// 
///   "","to ``invite``","to ``join``","to ``leave``","to ``ban``","to ``knock``"
///   "from ``invite``","No change.","User joined the room.","If the ``state_key`` is the same as the ``sender``, the user rejected the invite. Otherwise, the ``state_key`` user had their invite revoked.","User was banned.","Not implemented."
///   "from ``join``","Must never happen.","``displayname`` or ``avatar_url`` changed.","If the ``state_key`` is the same as the ``sender``, the user left. Otherwise, the ``state_key`` user was kicked.","User was kicked and banned.","Not implemented."
///   "from ``leave``","New invitation sent.","User joined.","No change.","User was banned.","Not implemented."
///   "from ``ban``","Must never happen.","Must never happen.","User was unbanned.","No change.","Not implemented."
///   "from ``knock``","Not implemented.","Not implemented.","Not implemented.","Not implemented.","Not implemented."
    std::string type;

/// Adjusts the membership state for a user in a room. It is preferable to use the membership APIs (``/rooms/<room id>/invite`` etc) when performing membership actions rather than adjusting the state directly as there are a restricted set of valid transformations. For example, user A cannot force user B to join a room, and trying to force this state change directly will fail.
/// 
/// The following membership states are specified:
/// 
/// - ``invite`` - The user has been invited to join a room, but has not yet joined it. They may not participate in the room until they join.
/// 
/// - ``join`` - The user has joined the room (possibly after accepting an invite), and may participate in it.
/// 
/// - ``leave`` - The user was once joined to the room, but has since left (possibly by choice, or possibly by being kicked).
/// 
/// - ``ban`` - The user has been banned from the room, and is no longer allowed to join it until they are un-banned from the room (by having their membership state set to a value other than ``ban``).
/// 
/// - ``knock`` - This is a reserved word, which currently has no meaning.
/// 
/// The ``third_party_invite`` property will be set if this invite is an ``invite`` event and is the successor of an ``m.room.third_party_invite`` event, and absent otherwise.
/// 
/// This event may also include an ``invite_room_state`` key inside the event's ``unsigned`` data.
/// If present, this contains an array of ``StrippedState`` Events. These events provide information
/// on a subset of state events such as the room name.
/// 
/// The user for which a membership applies is represented by the ``state_key``. Under some conditions,
/// the ``sender`` and ``state_key`` may not match - this may be interpreted as the ``sender`` affecting
/// the membership state of the ``state_key`` user.
/// 
/// The ``membership`` for a given user can change over time. The table below represents the various changes
/// over time and how clients and servers must interpret those changes. Previous membership can be retrieved
/// from the ``prev_content`` object on an event. If not present, the user's previous membership must be assumed
/// as ``leave``.
/// 
/// .. TODO: Improve how this table is written? We use a csv-table to get around vertical header restrictions.
/// 
/// .. csv-table::
///   :header-rows: 1
///   :stub-columns: 1
/// 
///   "","to ``invite``","to ``join``","to ``leave``","to ``ban``","to ``knock``"
///   "from ``invite``","No change.","User joined the room.","If the ``state_key`` is the same as the ``sender``, the user rejected the invite. Otherwise, the ``state_key`` user had their invite revoked.","User was banned.","Not implemented."
///   "from ``join``","Must never happen.","``displayname`` or ``avatar_url`` changed.","If the ``state_key`` is the same as the ``sender``, the user left. Otherwise, the ``state_key`` user was kicked.","User was kicked and banned.","Not implemented."
///   "from ``leave``","New invitation sent.","User joined.","No change.","User was banned.","Not implemented."
///   "from ``ban``","Must never happen.","Must never happen.","User was unbanned.","No change.","Not implemented."
///   "from ``knock``","Not implemented.","Not implemented.","Not implemented.","Not implemented.","Not implemented."
    JsonWrap unsignedData;
};

}
namespace nlohmann
{
using namespace Kazv;
template<>
struct adl_serializer<TheCurrentMembershipStateOfAUserInTheRoom> {
  static void to_json(json& jo, const TheCurrentMembershipStateOfAUserInTheRoom &pod)
  {
    jo = static_cast<const JsonWrap &>(pod);
  
    
    addToJsonIfNeeded(jo, "content"s, pod.content);
    
    addToJsonIfNeeded(jo, "state_key"s, pod.stateKey);
    
    addToJsonIfNeeded(jo, "type"s, pod.type);
    
    addToJsonIfNeeded(jo, "unsigned"s, pod.unsignedData);
  }
  static void from_json(const json &jo, TheCurrentMembershipStateOfAUserInTheRoom& result)
  {
    static_cast<JsonWrap &>(result) = jo;
    if (jo.contains("content"s)) {
      result.content = jo.at("content"s);
    }
    if (jo.contains("state_key"s)) {
      result.stateKey = jo.at("state_key"s);
    }
    if (jo.contains("type"s)) {
      result.type = jo.at("type"s);
    }
    if (jo.contains("unsigned"s)) {
      result.unsignedData = jo.at("unsigned"s);
    }
  
  }
};
    }

    namespace Kazv
    {
} // namespace Kazv
