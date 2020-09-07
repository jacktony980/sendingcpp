
#include <regex>
#include <tuple>
#include <cereal/archives/json.hpp>
#include <lager/lenses/optional.hpp>

#include <zug/transducer/filter.hpp>
#include <zug/into.hpp>
#include <zug/compose.hpp>
#include <immer/flex_vector_transient.hpp>
#include <immer/flex_vector.hpp>

#include "commands.hpp"

using namespace std::string_literals;

static std::regex roomMsgsRegex("room msgs (.+)");
static std::regex roomStatesRegex("room states (.+)");
static std::regex roomMemsRegex("room mems (.+)");

lager::reader<Kazv::Room> roomFor(lager::reader<Kazv::Client> c,
                                  std::string roomId) {
    return c[&Kazv::Client::roomList]
        [&Kazv::RoomList::rooms]
        [roomId]
        [lager::lenses::or_default];
}


std::optional<Kazv::Client::Action> intent(std::string l, lager::reader<Kazv::Client> c)
{
    std::smatch m;
    if (l == "rooms") {
        lager::reader<immer::map<std::string, Kazv::Room>> rooms =
            c[&Kazv::Client::roomList]
            [&Kazv::RoomList::rooms];

        std::cout << "Room Id\tRoom Name\n";
        for (auto [id, room] : rooms.get()) {
            Kazv::KeyOfState nameKey{"m.room.name"s, ""s};
            auto ev = room.stateEvents[nameKey];
            Kazv::json content = ev.content();
            std::string roomName = "<no name>";
            if (content.contains("name"s)) {
                roomName = content["name"s];
            }
            std::cout << id << "\t" << roomName << "\n";
        }

        return std::nullopt;
    } else if (std::regex_match(l, m, roomMsgsRegex)) {
        auto roomId = m[1].str();
        lager::reader<Kazv::Room> room = roomFor(c, roomId);

        auto msgs = room.get().timeline;

        std::cout << "Messages in " << roomId << ":\n";
        for (auto msg : msgs) {
            std::cout << "Event "
                      << msg.id() << " from "
                      << msg.sender() << " at "
                      << msg.originServerTs() << " typed "
                      << msg.type() << "\n";
            {
                cereal::JSONOutputArchive archive( std::cout );
                archive(msg.content());
            }
            std::cout << std::endl;
        }

        return std::nullopt;
    } else if (std::regex_match(l, m, roomMemsRegex)) {
        auto roomId = m[1].str();
        lager::reader<Kazv::Room> room = roomFor(c, roomId);

        using MemberNode = std::pair<std::string, Kazv::Event>;
        auto memberEvents =
            zug::into(
                immer::flex_vector_transient<MemberNode>{},
                zug::filter(
                    [](auto val) {
                        auto [k, v] = val;
                        auto [type, stateKey] = k;
                        return type == "m.room.member"s;
                    })
                | zug::map(
                    [](auto val) {
                        auto [k, v] = val;
                        auto [type, stateKey] = k;
                        return MemberNode{stateKey, v};
                    })
                | zug::filter(
                    [](auto val) {
                        auto [stateKey, ev] = val;
                        return ev.content().get()
                            .at("membership"s) == "join"s;
                    }),
                room.get().stateEvents)
            .persistent();

        std::cout << "Members in " << roomId << ":\n";
        for (auto [userId, a] : memberEvents) {
            std::cout << userId << std::endl;
        }

        return std::nullopt;
    } else if (std::regex_match(l, m, roomStatesRegex)) {
        auto roomId = m[1].str();
        auto room = roomFor(c, roomId);
        auto states = room.get().stateEvents;

        std::cout << "States in " << roomId << ":\n";
        for (auto [k, st] : states) {
            auto [type, stateKey] = k;
            std::cout << "State typed "
                      << type << " with stateKey "
                      << stateKey << "\n";
            {
                cereal::JSONOutputArchive archive( std::cout );
                archive(st);
            }
            std::cout << std::endl;
        }

        return std::nullopt;
    } else {
        // no valid action, display help
        std::cout << "Commands:\n"
                  << "rooms -- List rooms\n"
                  << "room msgs <roomId> -- List room messages\n"
                  << "room states <roomId> -- List room states\n"
                  << "room mems <roomId> -- List room members\n\n";
        return std::nullopt;
    }
}
