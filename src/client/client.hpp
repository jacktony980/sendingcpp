/*
 * Copyright (C) 2020 Tusooa Zhu
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

#pragma once
#include <libkazv-config.hpp>

#include <lager/reader.hpp>
#include <immer/box.hpp>
#include <immer/map.hpp>
#include <immer/flex_vector.hpp>
#include <immer/flex_vector_transient.hpp>

#include "sdk-model.hpp"
#include "client/client-model.hpp"
#include "client/actions/content.hpp"

#include "room/room.hpp"

namespace Kazv
{
    class Client
    {
    public:
        Client(lager::reader<SdkModel> sdk,
               Context<ClientAction> ctx);

        /* lager::reader<immer::map<std::string, Room>> */
        inline auto rooms() const {
            return m_client
                [&ClientModel::roomList]
                [&RoomListModel::rooms];
        }

        /* lager::reader<RangeT<std::string>> */
        inline auto roomIds() const {
            return rooms().xform(
                zug::map([](auto m) {
                             return intoImmer(
                                 immer::flex_vector<std::string>{},
                                 zug::map([](auto val) { return val.first; }),
                                 m);
                         }));
        }

        KAZV_WRAP_ATTR(ClientModel, m_client, serverUrl)
        KAZV_WRAP_ATTR(ClientModel, m_client, loggedIn)
        KAZV_WRAP_ATTR(ClientModel, m_client, userId)
        KAZV_WRAP_ATTR(ClientModel, m_client, token)
        KAZV_WRAP_ATTR(ClientModel, m_client, deviceId)
        KAZV_WRAP_ATTR(ClientModel, m_client, toDevice)

        Room room(std::string id) const;

        Room roomByCursor(lager::reader<std::string> id) const;

        BoolPromise passwordLogin(std::string homeserver, std::string username,
                                  std::string password, std::string deviceName) const;

        BoolPromise tokenLogin(std::string homeserver, std::string username,
                               std::string token, std::string deviceId) const;

        BoolPromise createRoom(RoomVisibility v,
                               std::optional<std::string> name = {},
                               std::optional<std::string> alias = {},
                               immer::array<std::string> invite = {},
                               std::optional<bool> isDirect = {},
                               bool allowFederate = true,
                               std::optional<std::string> topic = {},
                               JsonWrap powerLevelContentOverride = json::object()) const;

        BoolPromise joinRoomById(std::string roomId) const;

        BoolPromise joinRoom(std::string roomId, immer::array<std::string> serverName) const;

        BoolPromise uploadContent(immer::box<Bytes> content,
                                  std::string uploadId,
                                  std::optional<std::string> filename = std::nullopt,
                                  std::optional<std::string> contentType = std::nullopt) const;

        inline std::string mxcUriToHttp(std::string mxcUri) const {
            using namespace CursorOp;
            auto [serverName, mediaId] = mxcUriToMediaDesc(mxcUri);
            return (+m_client)
                .job<GetContentJob>()
                .make(serverName, mediaId).url();
        }

        BoolPromise downloadContent(std::string mxcUri) const;

        BoolPromise downloadThumbnail(std::string mxcUri,
                                      int width,
                                      int height,
                                      std::optional<ThumbnailResizingMethod> method = std::nullopt) const;

        // lager::reader<bool>
        inline auto syncing() const {
            return m_client[&ClientModel::syncing];
        }

        BoolPromise startSyncing() const;

    private:
        lager::reader<SdkModel> m_sdk;
        lager::reader<ClientModel> m_client;
        Context<ClientAction> m_ctx;
    };

}
