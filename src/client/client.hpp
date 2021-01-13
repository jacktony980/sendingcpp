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

#include <lager/reader.hpp>
#include <immer/box.hpp>
#include <immer/map.hpp>
#include <immer/flex_vector.hpp>
#include <immer/flex_vector_transient.hpp>

#include "client/client-model.hpp"
#include "client/actions/content.hpp"

#include "room/room.hpp"

namespace Kazv
{
    class Client
    {
    public:
        inline Client(lager::reader<ClientModel> client,
                      lager::context<ClientAction> ctx)
            : m_client(std::move(client))
            , m_ctx(std::move(ctx)) {}

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

        /* Room */
        inline auto room(std::string id) const {
            return Room(rooms()[std::move(id)]
                        [lager::lenses::or_default].make(),
                        m_ctx);
        }

        inline auto roomByCursor(lager::reader<std::string> id) const {
            return Room(lager::with(rooms(), id)
                        .xform(zug::map([](auto rooms, auto id) {
                                            return rooms[id];
                                        })).make(),
                        m_ctx);
        }

        inline void passwordLogin(std::string homeserver, std::string username,
                                  std::string password, std::string deviceName) const {
            m_ctx.dispatch(LoginAction{
                    homeserver, username, password, deviceName});
        }

        inline void tokenLogin(std::string homeserver, std::string username,
                               std::string token, std::string deviceId) const {
            m_ctx.dispatch(TokenLoginAction{
                    homeserver, username, token, deviceId});
        }

        inline void createRoom(RoomVisibility v,
                               std::optional<std::string> name = {},
                               std::optional<std::string> alias = {},
                               immer::array<std::string> invite = {},
                               std::optional<bool> isDirect = {},
                               bool allowFederate = true,
                               std::optional<std::string> topic = {},
                               JsonWrap powerLevelContentOverride = json::object()) const {
            CreateRoomAction a;
            a.visibility = v;
            a.name = name;
            a.roomAliasName = alias;
            a.invite = invite;
            a.isDirect = isDirect;
            a.topic = topic;
            a.powerLevelContentOverride = powerLevelContentOverride;
            // Synapse won't buy it if we do not provide
            // a creationContent object.
            a.creationContent = json{
                {"m.federate", allowFederate}
            };
            m_ctx.dispatch(std::move(a));
        }

        inline void joinRoomById(std::string roomId) const {
            m_ctx.dispatch(JoinRoomByIdAction{roomId});
        }

        inline void joinRoom(std::string roomId, immer::array<std::string> serverName) const {
            m_ctx.dispatch(JoinRoomAction{roomId, serverName});
        }

        inline void uploadContent(immer::box<Bytes> content,
                                  std::string uploadId,
                                  std::optional<std::string> filename = std::nullopt,
                                  std::optional<std::string> contentType = std::nullopt) const {
            m_ctx.dispatch(UploadContentAction{content, filename, contentType, uploadId});
        }

        inline std::string mxcUriToHttp(std::string mxcUri) const {
            using namespace CursorOp;
            auto [serverName, mediaId] = mxcUriToMediaDesc(mxcUri);
            return (+m_client)
                .job<GetContentJob>()
                .make(serverName, mediaId).url();
        }

        inline void downloadContent(std::string mxcUri) const {
            m_ctx.dispatch(DownloadContentAction{mxcUri});
        }

        inline void downloadThumbnail(std::string mxcUri,
                                      int width,
                                      int height,
                                      std::optional<ThumbnailResizingMethod> method = std::nullopt) const {
            m_ctx.dispatch(DownloadThumbnailAction{mxcUri, width, height, method, std::nullopt});
        }

        // lager::reader<bool>
        inline auto syncing() const {
            return m_client[&ClientModel::syncing];
        }

        inline void startSyncing() const {
            using namespace Kazv::CursorOp;

            if (+syncing()) {
                return;
            }

            // filters are incomplete
            if ((+m_client[&ClientModel::initialSyncFilterId]).empty()
                || (+m_client[&ClientModel::incrementalSyncFilterId]).empty()) {
                m_ctx.dispatch(PostInitialFiltersAction{});
            } else { // sync is just interrupted
                m_ctx.dispatch(SyncAction{});
            }
        }

    private:
        lager::reader<ClientModel> m_client;
        lager::context<ClientAction> m_ctx;
    };

}
