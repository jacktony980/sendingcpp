/******************************************************************************
 * THIS FILE IS GENERATED - ANY EDITS WILL BE OVERWRITTEN
 */

#pragma once

#include "types.hpp"


namespace Kazv {

struct RequestMsisdnValidation
{       

/// A unique string generated by the client, and used to identify the
/// validation attempt. It must be a string consisting of the characters
/// ``[0-9a-zA-Z.=_-]``. Its length must not exceed 255 characters and it
/// must not be empty.
    std::string clientSecret;

/// The two-letter uppercase ISO-3166-1 alpha-2 country code that the
/// number in ``phone_number`` should be parsed as if it were dialled from.
    std::string country;

/// The phone number to validate.
    std::string phoneNumber;

/// The server will only send an SMS if the ``send_attempt`` is a
/// number greater than the most recent one which it has seen,
/// scoped to that ``country`` + ``phone_number`` + ``client_secret``
/// triple. This is to avoid repeatedly sending the same SMS in
/// the case of request retries between the POSTing user and the
/// identity server. The client should increment this value if
/// they desire a new SMS (e.g. a reminder) to be sent.
    int sendAttempt;

/// Optional. When the validation is completed, the identity server will
/// redirect the user to this URL. This option is ignored when submitting
/// 3PID validation information through a POST request.
    std::string nextLink;
};

}
namespace nlohmann
{
using namespace Kazv;
template<>
struct adl_serializer<RequestMsisdnValidation> {
  static void to_json(json& jo, const RequestMsisdnValidation &pod)
  {
  
  
    jo["client_secret"s] = pod.clientSecret;
    
    jo["country"s] = pod.country;
    
    jo["phone_number"s] = pod.phoneNumber;
    
    jo["send_attempt"s] = pod.sendAttempt;
    
    
    addToJsonIfNeeded(jo, "next_link"s, pod.nextLink);
  }
  static void from_json(const json &jo, RequestMsisdnValidation& result)
  {
  
    if (jo.contains("client_secret"s)) {
      result.clientSecret = jo.at("client_secret"s);
    }
    if (jo.contains("country"s)) {
      result.country = jo.at("country"s);
    }
    if (jo.contains("phone_number"s)) {
      result.phoneNumber = jo.at("phone_number"s);
    }
    if (jo.contains("send_attempt"s)) {
      result.sendAttempt = jo.at("send_attempt"s);
    }
    if (jo.contains("next_link"s)) {
      result.nextLink = jo.at("next_link"s);
    }
  
  }
};
    }

    namespace Kazv
    {
} // namespace Kazv
