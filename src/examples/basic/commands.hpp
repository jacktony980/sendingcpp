
#pragma once
#include <optional>
#include <string>
#include <lager/reader.hpp>

#include <client/client.hpp>


std::optional<Kazv::Client::Action> intent(std::string l, lager::reader<Kazv::Client> c);
