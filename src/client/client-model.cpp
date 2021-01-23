/*
 * Copyright (C) 2020 Tusooa Zhu <tusooa@vista.aero>
 *
 * This file is part of libkazv.
 *
 * libkazv is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as
 * published by the Free Software Foundation, either version 3 of the
 * License, or (at your option) any later version.
 *
 * libkazv is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with libkazv.  If not, see <https://www.gnu.org/licenses/>.
 */

#include <libkazv-config.hpp>


#include <lager/util.hpp>
#include <lager/context.hpp>
#include <functional>

#include <zug/transducer/filter.hpp>

#include <immer/flex_vector_transient.hpp>

#include "debug.hpp"

#include "client-model.hpp"

#include "actions/states.hpp"
#include "actions/auth.hpp"
#include "actions/membership.hpp"
#include "actions/paginate.hpp"
#include "actions/send.hpp"
#include "actions/states.hpp"
#include "actions/sync.hpp"
#include "actions/ephemeral.hpp"
#include "actions/content.hpp"
#include "actions/encryption.hpp"

namespace Kazv
{
    auto ClientModel::update(ClientModel m, Action a) -> Result
    {
        auto oldDeviceLists = m.deviceLists;

        auto [newClient, effect] = lager::match(std::move(a))(
            [&](Error::Action a) -> Result {
                m.error = Error::update(m.error, a);
                return {std::move(m), lager::noop};
            },

            [&](RoomListAction a) -> Result {
                m.roomList = RoomListModel::update(std::move(m.roomList), a);
                return {std::move(m), lager::noop};
            },
            [&](ResubmitJobAction a) -> Result {
                m.addJob(std::move(a.job));
                return { std::move(m), lager::noop };
            },
            [&](auto a) -> decltype(updateClient(m, a)) {
                return updateClient(m, a);
            },
#define RESPONSE_FOR(_jobId)                                            \
            if (r.jobId() == #_jobId) {                                 \
                return processResponse(m, _jobId##Response{std::move(r)}); \
            }

            [&](ProcessResponseAction a) -> Result {
                auto r = std::move(a.response);

                // auth
                RESPONSE_FOR(Login);
                // paginate
                RESPONSE_FOR(GetRoomEvents);
                // sync
                RESPONSE_FOR(Sync);
                RESPONSE_FOR(DefineFilter);
                // membership
                RESPONSE_FOR(CreateRoom);
                RESPONSE_FOR(InviteUser);
                RESPONSE_FOR(JoinRoomById);
                RESPONSE_FOR(JoinRoom);
                RESPONSE_FOR(LeaveRoom);
                RESPONSE_FOR(ForgetRoom);
                // send
                RESPONSE_FOR(SendMessage);
                RESPONSE_FOR(SendToDevice);
                // states
                RESPONSE_FOR(GetRoomState);
                RESPONSE_FOR(SetRoomStateWithKey);
                RESPONSE_FOR(GetRoomStateWithKey);
                // ephemeral
                RESPONSE_FOR(SetTyping);
                RESPONSE_FOR(PostReceipt);
                RESPONSE_FOR(SetReadMarker);
                // content
                RESPONSE_FOR(UploadContent);
                RESPONSE_FOR(GetContent);
                RESPONSE_FOR(GetContentThumbnail);
                // encryption
                RESPONSE_FOR(UploadKeys);
                RESPONSE_FOR(QueryKeys);
                RESPONSE_FOR(ClaimKeys);

                m.addTrigger(UnrecognizedResponse{std::move(r)});
                return { std::move(m), lager::noop };
            }

#undef RESPONSE_FOR
            );

        // Rotate megolm keys for rooms whose users' device list has changed
        auto changedUsers = newClient.deviceLists.diff(oldDeviceLists);
        if (! changedUsers.empty()) {
            for (auto [roomId, room] : newClient.roomList.rooms) {
                auto it = std::find_if(changedUsers.begin(), changedUsers.end(),
                                       [=](auto userId) { return room.hasUser(userId); });
                if (it != changedUsers.end()) {
                    newClient.roomList.rooms =
                        std::move(newClient.roomList.rooms)
                        .update(roomId, [](auto room) {
                                            room.shouldRotateSessionKey = true;
                                            return room;
                                        });
                }
            }
        }

        return { std::move(newClient), std::move(effect) };
    }

    std::pair<Event, std::optional<std::string>> ClientModel::megOlmEncrypt(Event e, std::string roomId)
    {
        if (!crypto) {
            kzo.client.dbg() << "We do not have e2ee, so do not encrypt events" << std::endl;
            return { e, std::nullopt };
        }

        if (e.encrypted()) {
            kzo.client.dbg() << "The event is already encrypted. Ignoring it." << std::endl;
            return { e, std::nullopt };
        }

        auto &c = crypto.value();

        auto j = e.originalJson().get();
        auto r = roomList[roomId];

        if (! r.encrypted) {
            kzo.client.dbg() << "The room " << roomId
                             << " is not encrypted, so do not encrypt events" << std::endl;
            return { e, std::nullopt };
        }

        auto desc = r.sessionRotateDesc();

        auto keyOpt = std::optional<std::string>{};
        if (r.shouldRotateSessionKey) {
            kzo.client.dbg() << "We should rotate this session." << std::endl;
            keyOpt = c.rotateMegOlmSession(roomId);
        } else {
            keyOpt = c.rotateMegOlmSessionIfNeeded(roomId, desc);
        }

        // we no longer need to rotate session
        // until next time a device change happens
        roomList.rooms = std::move(roomList.rooms)
            .update(roomId, [](auto r) { r.shouldRotateSessionKey = false; return r; });

        // so that Crypto::encryptMegOlm() can find room id
        j["room_id"] = roomId;

        auto content = c.encryptMegOlm(j);
        j["type"] = "m.room.encrypted";
        j["content"] = std::move(content);
        j["content"]["device_id"] = deviceId;

        kzo.client.dbg() << "Encrypted json is " << j.dump() << std::endl;
        kzo.client.dbg() << "Session key is " << (keyOpt ? keyOpt.value() : "<not rotated>") << std::endl;

        return { Event(JsonWrap(j)), keyOpt };
    }

    Event ClientModel::olmEncrypt(Event e, immer::map<std::string, immer::flex_vector<std::string>> userIdToDeviceIdMap)
    {
        if (!crypto) {
            kzo.client.dbg() << "We do not have e2ee, so do not encrypt events" << std::endl;
            return e;
        }

        if (e.encrypted()) {
            kzo.client.dbg() << "The event is already encrypted. Ignoring it." << std::endl;
            return e;
        }

        auto &c = crypto.value();

        auto origJson = e.originalJson().get();

        auto encJson = json::object();
        encJson["content"] = json{
            {"algorithm", CryptoConstants::olmAlgo},
            {"ciphertext", json::object()},
            {"sender_key", c.curve25519IdentityKey()},
        };

        encJson["type"] = "m.room.encrypted";

        for (auto [userId, devices] : userIdToDeviceIdMap) {
            for (auto dev : devices) {
                auto devInfoOpt = deviceLists.get(userId, dev);
                if (! devInfoOpt) {
                    continue;
                }
                auto devInfo = devInfoOpt.value();
                auto jsonForThisDevice = origJson;
                jsonForThisDevice["sender"] = this->userId;
                jsonForThisDevice["recipient"] = userId;
                jsonForThisDevice["recipient_keys"] = json{
                    {CryptoConstants::ed25519, devInfo.ed25519Key}
                };
                jsonForThisDevice["keys"] = json{
                    {CryptoConstants::ed25519, c.ed25519IdentityKey()}
                };
                encJson["content"]["ciphertext"]
                    .merge_patch(c.encryptOlm(jsonForThisDevice, devInfo.curve25519Key));
            }
        }

        return Event(JsonWrap(encJson));
    }

    immer::flex_vector<std::string /* deviceId */> ClientModel::devicesToSendKeys(std::string userId) const
    {
        auto trustLevelNeeded = DeviceTrustLevel::Unseen;

        // XXX: preliminary approach
        auto shouldSendP = [=](auto deviceInfo, auto /* deviceMap */) {
                               return deviceInfo.trustLevel >= trustLevelNeeded;
                           };


        auto devices = deviceLists.devicesFor(userId);

        return intoImmer(
            immer::flex_vector<std::string>{},
            zug::filter([=](auto n) {
                         auto [id, dev] = n;
                         return shouldSendP(dev, devices);
                     })
            | zug::map([=](auto n) {
                           return n.first;
                       }),
            devices);
    }
}
