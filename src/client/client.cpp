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

#include <libkazv-config.hpp>

#include <lager/constant.hpp>

#include "client.hpp"

namespace Kazv
{
    Client::Client(lager::reader<SdkModel> sdk,
                   Context<ClientAction> ctx)
        : m_sdk(sdk)
        , m_client(sdk.map(&SdkModel::c))
        , m_ctx(std::move(ctx))
    {
    }

    Room Client::room(std::string id) const
    {
        return Room(m_sdk, lager::make_constant(id), m_ctx);
    }

    Room Client::roomByCursor(lager::reader<std::string> id) const
    {
        return Room(m_sdk, id, m_ctx);
    }

    auto Client::passwordLogin(std::string homeserver, std::string username,
                               std::string password, std::string deviceName) const
        -> PromiseT
    {
        return m_ctx.dispatch(LoginAction{
                homeserver, username, password, deviceName});
    }

    auto Client::tokenLogin(std::string homeserver, std::string username,
                            std::string token, std::string deviceId) const
        -> PromiseT
    {
        return m_ctx.dispatch(TokenLoginAction{
                homeserver, username, token, deviceId});
    }

    auto Client::createRoom(RoomVisibility v,
                            std::optional<std::string> name,
                            std::optional<std::string> alias,
                            immer::array<std::string> invite,
                            std::optional<bool> isDirect,
                            bool allowFederate,
                            std::optional<std::string> topic,
                            JsonWrap powerLevelContentOverride) const
        -> PromiseT
    {
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
        return m_ctx.dispatch(std::move(a));
    }

    auto Client::joinRoomById(std::string roomId) const -> PromiseT
    {
        return m_ctx.dispatch(JoinRoomByIdAction{roomId});
    }

    auto Client::joinRoom(std::string roomId, immer::array<std::string> serverName) const
        -> PromiseT
    {
        return m_ctx.dispatch(JoinRoomAction{roomId, serverName});
    }

    auto Client::uploadContent(immer::box<Bytes> content,
                              std::string uploadId,
                              std::optional<std::string> filename,
                              std::optional<std::string> contentType) const
        -> PromiseT
    {
        return m_ctx.dispatch(UploadContentAction{content, filename, contentType, uploadId});
    }

    auto Client::downloadContent(std::string mxcUri) const
        -> PromiseT
    {
        return m_ctx.dispatch(DownloadContentAction{mxcUri});
    }

    auto Client::downloadThumbnail(
        std::string mxcUri,
        int width,
        int height,
        std::optional<ThumbnailResizingMethod> method) const
        -> PromiseT
    {
        return m_ctx.dispatch(DownloadThumbnailAction{mxcUri, width, height, method, std::nullopt});
    }

    auto Client::startSyncing() const -> PromiseT
    {
        using namespace Kazv::CursorOp;

        if (+syncing()) {
            return m_ctx.createResolvedPromise(true);
        }

        // filters are incomplete
        if ((+m_client[&ClientModel::initialSyncFilterId]).empty()
            || (+m_client[&ClientModel::incrementalSyncFilterId]).empty()) {
            return m_ctx.dispatch(PostInitialFiltersAction{});
        } else if (+m_client[&ClientModel::crypto] // encryption is on
                   && ! +m_client[&ClientModel::identityKeysUploaded]) { // but identity keys are not published
            return m_ctx.dispatch(UploadIdentityKeysAction{});
        } else { // sync is just interrupted
            return m_ctx.dispatch(SyncAction{});
        }
    }
}
