/*
 * Copyright (C) 2020-2021 Tusooa Zhu <tusooa@kazv.moe>
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

#include <filesystem>

#include <lager/constant.hpp>

#include "client.hpp"

namespace Kazv
{
    Client::Client(lager::reader<SdkModel> sdk,
                   ContextT ctx, std::nullopt_t)
        : m_sdk(sdk)
        , m_client(sdk.map(&SdkModel::c))
        , m_ctx(std::move(ctx))
    {
    }

    Client::Client(lager::reader<SdkModel> sdk,
                   ContextWithDepsT ctx)
        : m_sdk(sdk)
        , m_client(sdk.map(&SdkModel::c))
        , m_ctx(ctx)
        , m_deps(std::move(ctx))
    {
    }

    Client::Client(InEventLoopTag,
                   ContextWithDepsT ctx)
        : m_sdk(std::nullopt)
        , m_client(std::nullopt)
        , m_ctx(ctx)
        , m_deps(std::move(ctx))
#ifdef KAZV_USE_THREAD_SAFETY_HELPER
        , KAZV_ON_EVENT_LOOP_VAR(true)
#endif
    {
    }


    Room Client::room(std::string id) const
    {
        return Room(sdkCursor(), lager::make_constant(id), m_ctx);
    }

    Room Client::roomByCursor(lager::reader<std::string> id) const
    {
        return Room(sdkCursor(), id, m_ctx);
    }

    auto Client::passwordLogin(std::string homeserver, std::string username,
                               std::string password, std::string deviceName) const
        -> PromiseT
    {
        auto p1 = m_ctx.dispatch(LoginAction{
                homeserver, username, password, deviceName});
        p1
            .then([*this](auto stat) {
                      if (! stat.success()) {
                          return;
                      }
                      // It is meaningless to wait for it in a Promise
                      // that is never exposed to the user.
                      startSyncing();
                  });

        return p1;
    }

    auto Client::tokenLogin(std::string homeserver, std::string username,
                            std::string token, std::string deviceId) const
        -> PromiseT
    {
        auto p1 = m_ctx.dispatch(TokenLoginAction{
                homeserver, username, token, deviceId});
        p1
            .then([*this](auto stat) {
                      if (! stat.success()) {
                          return;
                      }
                      startSyncing();
                  });

        return p1;
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
        return m_ctx.dispatch(UploadContentAction{
                FileDesc(FileContent{content.get().begin(), content.get().end()}),
                filename, contentType, uploadId});
    }

    auto Client::uploadContent(FileDesc file) const
        -> PromiseT
    {
        auto basename = file.name()
            ? std::optional(std::filesystem::path(file.name().value()).filename().native())
            : std::nullopt;
        return m_ctx.dispatch(UploadContentAction{
                file,
                // use only basename to prevent path info being leaked
                basename,
                file.contentType(),
                // uploadId unused
                std::string{}});
    }

    auto Client::downloadContent(std::string mxcUri, std::optional<FileDesc> downloadTo) const
        -> PromiseT
    {
        return m_ctx.dispatch(DownloadContentAction{mxcUri, downloadTo});
    }

    auto Client::downloadThumbnail(
        std::string mxcUri,
        int width,
        int height,
        std::optional<ThumbnailResizingMethod> method,
        std::optional<FileDesc> downloadTo) const
        -> PromiseT
    {
        return m_ctx.dispatch(DownloadThumbnailAction{mxcUri, width, height, method, std::nullopt, downloadTo});
    }

    auto Client::startSyncing() const -> PromiseT
    {
        KAZV_VERIFY_THREAD_ID();
        using namespace Kazv::CursorOp;

        if (+syncing()) {
            return m_ctx.createResolvedPromise(true);
        }

        auto p1 = m_ctx.createResolvedPromise(true)
            .then([*this](auto) {
                      // post filters, if filters are incomplete
                      if ((+clientCursor()[&ClientModel::initialSyncFilterId]).empty()
                          || (+clientCursor()[&ClientModel::incrementalSyncFilterId]).empty()) {
                          return m_ctx.dispatch(PostInitialFiltersAction{});
                      }
                      return m_ctx.createResolvedPromise(true);
                  })
            .then([*this](auto stat) {
                      if (! stat.success()) {
                          return m_ctx.createResolvedPromise(stat);
                      }
                      // Upload identity keys if we need to
                      if (+clientCursor()[&ClientModel::crypto]
                          && ! +clientCursor()[&ClientModel::identityKeysUploaded]) {
                          return m_ctx.dispatch(UploadIdentityKeysAction{});
                      } else {
                          return m_ctx.createResolvedPromise(true);
                      }
                  });

        p1
            .then([m_ctx=m_ctx](auto stat) {
                      m_ctx.dispatch(SetShouldSyncAction{true});
                      return stat;
                  })
            .then([*this](auto stat) {
                      if (stat.success()) {
                          syncForever();
                      }
                  });

        return p1;
    }

    auto Client::syncForever(std::optional<int> retryTime) const -> void
    {
        KAZV_VERIFY_THREAD_ID();

        // assert (m_deps);
        using namespace CursorOp;

        bool isInitialSync = ! (+clientCursor()[&ClientModel::syncToken]).has_value();

        bool shouldSync = +clientCursor()[&ClientModel::shouldSync];

        if (! shouldSync) {
            return;
        }
        //

        auto syncRes = m_ctx.dispatch(SyncAction{});

        auto uploadOneTimeKeysRes = syncRes
            .then([*this](auto stat) {
                      if (! stat.success()) {
                          return m_ctx.createResolvedPromise(stat);
                      }
                      bool hasCrypto{+clientCursor()[&ClientModel::crypto]};
                      auto p1 = hasCrypto
                          ? m_ctx.dispatch(GenerateAndUploadOneTimeKeysAction{})
                          : m_ctx.createResolvedPromise(true);
                      return p1;
                  });

        auto queryKeysRes = syncRes
            .then([*this, isInitialSync](auto stat) {
                      if (! stat.success()) {
                          return m_ctx.createResolvedPromise(stat);
                      }
                      bool hasCrypto{+clientCursor()[&ClientModel::crypto]};
                      return hasCrypto
                          ? m_ctx.dispatch(QueryKeysAction{isInitialSync})
                          : m_ctx.createResolvedPromise(true);
                  });

        m_ctx.promiseInterface()
            .all(std::vector<PromiseT>{uploadOneTimeKeysRes, queryKeysRes})
            .then([*this, retryTime](auto stat) {
                      if (stat.success()) {
                          syncForever(); // reset retry time
                      } else {
                          auto firstRetryTime = +clientCursor()[&ClientModel::firstRetryMs];
                          auto retryTimeFactor = +clientCursor()[&ClientModel::retryTimeFactor];
                          auto maxRetryTime = +clientCursor()[&ClientModel::maxRetryMs];
                          auto curRetryTime = retryTime ? retryTime.value() : firstRetryTime;
                          if (curRetryTime > maxRetryTime) { curRetryTime = maxRetryTime; }
                          auto nextRetryTime = curRetryTime * retryTimeFactor;

                          kzo.client.warn() << "Sync failed, retrying in " << curRetryTime << "ms" << std::endl;
                          auto &jh = getJobHandler(m_deps.value());
                          jh.setTimeout([*this, nextRetryTime]() { syncForever(nextRetryTime); },
                                        curRetryTime);
                      }
                  });
    }

    void Client::stopSyncing() const
    {
        m_ctx.dispatch(SetShouldSyncAction{false});
    }

    lager::reader<ClientModel> Client::clientCursor() const
    {
        KAZV_VERIFY_THREAD_ID();

        if (m_client.has_value()) {
            return m_client.value();
        } else {
            assert(m_deps.has_value());
            return lager::get<SdkModelCursorKey>(m_deps.value())->map(&SdkModel::c);
        }
    }

    const lager::reader<SdkModel> &Client::sdkCursor() const
    {
        KAZV_VERIFY_THREAD_ID();

        if (m_sdk.has_value()) {
            return m_sdk.value();
        } else {
            assert(m_deps.has_value());
            return *(lager::get<SdkModelCursorKey>(m_deps.value()));
        }
    }

}
