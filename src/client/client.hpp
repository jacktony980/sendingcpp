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
#include "sdk-model-cursor-tag.hpp"

#include "room/room.hpp"

namespace Kazv
{
    /**
     * Represent a Matrix client.
     *
     * If the Client is constructed from a cursor originated from
     * a root whose event loop is on thread A, then we say that
     * the Client belongs to thread A. If the Client is not constructed
     * from a cursor, then we say that the Client belongs to the thread
     * where the event loop of the context runs.
     *
     * All methods in this class that take a cursor only take a cursor
     * on the same thread as the Client. All methods in this class that
     * return a cursor will return a cursor on the same thread as the Client.
     *
     * All methods in this class must be run on the same thread as the
     * the Client. If the Client is not constructed from a cursor,
     * copy-constructing another Client from this is safe from any thread.
     * If the Client is constructed from a cursor, copy-constructing another
     * Client is safe only from the same thread as this Client.
     */
    class Client
    {
    public:
        using ActionT = ClientAction;

        using DepsT = lager::deps<JobInterface &, EventInterface &, SdkModelCursorKey>;
        using ContextT = Context<ActionT>;
        using ContextWithDepsT = Context<ActionT, DepsT>;

        using PromiseT = SingleTypePromise<DefaultRetType>;
        /**
         * Constructor.
         *
         * Construct the client. Without Deps support.
         *
         * @warning You should not use this directly. Use
         * Sdk::client() instead.
         */
        Client(lager::reader<SdkModel> sdk,
               ContextT ctx, std::nullopt_t);

        /**
         * Constructor.
         *
         * Construct the client, with Deps support.
         *
         * This enables startSyncing() to work properly.
         *
         * @warning You should not use this directly. Use
         * Sdk::client() instead.
         */
        Client(lager::reader<SdkModel> sdk,
               ContextWithDepsT ctx);


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

        /**
         * Get the room with @c id .
         *
         * This is equivalent to `roomByCursor(lager::make_constant(id))`.
         *
         * @param id The room id.
         * @return A Room representing the room with `id`.
         */
        Room room(std::string id) const;

        /**
         * Get the room with `id`.
         *
         * The Room returned will change as the content in `id` changes.
         *
         * For example, you can have the Room that is always the first
         * alphabetically in all rooms by:
         *
         * \code{.cpp}
         * auto someProcessing =
         *     zug::map([=](auto ids) {
         *         std::sort(ids.begin(), ids.end(), [=](auto id1, auto id2) {
         *             using namespace Kazv::CursorOp;
         *             return (+client.room(id1).name()) < (+client.room(id2).name());
         *         });
         *         return ids;
         *     });
         * auto room =
         *     client.roomByCursor(
         *         client.roomIds().xform(someProcessing)[0]);
         * \endcode
         *
         * @param id A lager::reader<std::string> containing the room id.
         * @return A Room representing the room with `id`.
         */
        Room roomByCursor(lager::reader<std::string> id) const;

        /**
         * Login using the password.
         *
         * This will create a new session on the homeserver.
         *
         * If the returned Promise resolves successfully, this will
         * call `startSyncing()`.
         *
         * @param homeserver The base url of the homeserver. E.g. `https://tusooa.xyz`.
         * @param username The username. This can be the full user id or
         * just the local part. E.g. `tusooa`, `@tusooa:tusooa.xyz`.
         * @param password The password.
         * @param deviceName Optionally, a custom device name. If empty, `libkazv`
         * will be used.
         * @return A Promise that resolves when logging in successfully, or
         * when there is an error.
         */
        PromiseT passwordLogin(std::string homeserver, std::string username,
                               std::string password, std::string deviceName) const;

        /**
         * Login using `token` and `deviceId`.
         *
         * This will not make a request. Library users should make sure
         * the information is correct and the token and the device id are valid.
         *
         * If the returned Promise resolves successfully, this will
         * call `startSyncing()`.
         *
         * @param homeserver The base url of the homeserver. E.g. `https://tusooa.xyz`.
         * @param username The full user id. E.g. `@tusooa:tusooa.xyz`.
         * @param token The access token.
         * @param deviceId The device id that is paired with `token`.
         * @return A Promise that resolves when the account information is filled in.
         */
        PromiseT tokenLogin(std::string homeserver, std::string username,
                            std::string token, std::string deviceId) const;

        /**
         * Create a room.
         *
         * @param v The visibility of the room.
         * @param name The name of the room.
         * @param alias The alias of the room.
         * @param invite User ids to invite to this room.
         * @param isDirect Whether this room is a direct chat.
         * @param allowFederate Whether to allow users from other homeservers
         * to join this room.
         * @param topic The topic of the room.
         * @param powerLevelContentOverride The content of the m.room.power_levels
         * state event to override the default.
         * @return A Promise that resolves when the room is created,
         * or when there is an error.
         */
        PromiseT createRoom(RoomVisibility v,
                            std::optional<std::string> name = {},
                            std::optional<std::string> alias = {},
                            immer::array<std::string> invite = {},
                            std::optional<bool> isDirect = {},
                            bool allowFederate = true,
                            std::optional<std::string> topic = {},
                            JsonWrap powerLevelContentOverride = json::object()) const;

        /**
         * Join a room by its id.
         *
         * @param roomId The id of the room to join.
         * @return A Promise that resolves when the room is joined,
         * or when there is an error.
         */
        PromiseT joinRoomById(std::string roomId) const;

        /**
         * Join a room by its id or alias.
         *
         * @param roomId The id *or alias* of the room to join.
         * @param serverName A list of servers to use when joining the room.
         * This corresponds to the `via` parameter in a matrix.to url.
         * @return A Promise that resolves when the room is joined,
         * or when there is an error.
         */
        PromiseT joinRoom(std::string roomId, immer::array<std::string> serverName) const;

        /**
         * Upload content to the content repository.
         *
         * @param content The content to upload.
         * @param uploadId
         * @param filename The name of the file.
         * @param contentType The content type of the file.
         * @return A Promise that resolves when the upload is successful,
         * or when there is an error. If it successfully resolves to `r`,
         * `r.dataStr("mxcUri")` will be the MXC URI of the uploaded
         * content.
         */
        PromiseT uploadContent(immer::box<Bytes> content,
                               std::string uploadId,
                               std::optional<std::string> filename = std::nullopt,
                               std::optional<std::string> contentType = std::nullopt) const;

        /**
         * Upload content to the content repository.
         *
         * @param file The file to upload.
         * @return A Promise that resolves when the upload is successful,
         * or when there is an error. If it successfully resolves to `r`,
         * `r.dataStr("mxcUri")` will be the MXC URI of the uploaded
         * content.
         */
        PromiseT uploadContent(FileDesc file) const;


        /**
         * Convert a MXC URI to an HTTP(s) URI.
         *
         * The converted URI will be using the homeserver of
         * this Client.
         *
         * @param mxcUri The MXC URI to convert.
         * @return The HTTP(s) URI that has the content indicated
         * by `mxcUri`.
         */
        inline std::string mxcUriToHttp(std::string mxcUri) const {
            using namespace CursorOp;
            auto [serverName, mediaId] = mxcUriToMediaDesc(mxcUri);
            return (+m_client)
                .job<GetContentJob>()
                .make(serverName, mediaId).url();
        }

        /**
         * Download content from the content repository
         *
         * After the returned Promise resolves successfully,
         * if @c downloadTo is provided, the content will be available
         * in that file; if it is not provided, `r.dataStr("content")`
         * will contain the content of the downloaded file.
         *
         * @param mxcUri The MXC URI of the content.
         * @param downloadTo The file to write the content to. Must not be
         * an in-memory file.
         * @return A Promise that is resolved after the content
         * is downloaded, or when there is an error.
         */
        PromiseT downloadContent(std::string mxcUri,
                                 std::optional<FileDesc> downloadTo = std::nullopt) const;

        /**
         * Download a thumbnail from the content repository
         *
         * After the returned Promise resolves successfully,
         * if @c downloadTo is provided, the content will be available
         * in that file; if it is not provided, `r.dataStr("content")`
         * will contain the content of the downloaded file.
         *
         * @param mxcUri The MXC URI of the content.
         * @param width,height The dimension wanted for the thumbnail
         * @param method The method to generate the thumbnail. Either `Crop`
         * or `Scale`.
         * @param downloadTo The file to write the content to. Must not be
         * an in-memory file.
         * @return A Promise that is resolved after the thumbnail
         * is downloaded, or when there is an error.
         */
        PromiseT downloadThumbnail(std::string mxcUri,
                                   int width,
                                   int height,
                                   std::optional<ThumbnailResizingMethod> method = std::nullopt,
                                   std::optional<FileDesc> downloadTo = std::nullopt) const;

        // lager::reader<bool>
        inline auto syncing() const {
            return m_client[&ClientModel::syncing];
        }

        /**
         * Start syncing if the Client is not syncing.
         *
         * Syncing will continue indefinitely, if the preparation of
         * the sync (posting filters and uploading identity keys,
         * if needed) is successful, or until stopSyncing() is called.
         *
         * @return A Promise that resolves when the Client is syncing
         * (more exactly, when syncing() contains true), or when there
         * is an error in the preparation of the sync.
         */
        PromiseT startSyncing() const;

        /**
         * Stop the indefinite syncing.
         *
         * After this, no more syncing actions will be dispatched.
         */
        void stopSyncing() const;

        /**
         * Serialize the model to a Boost.Serialization archive.
         *
         * @param ar A Boost.Serialization output archive.
         *
         * This function can be used to save the model. For loading,
         * you should use the makeSdk function. For example:
         *
         * ```c++
         * client.serializeTo(outputAr);
         *
         * SdkModel m;
         * inputAr >> m;
         * auto newSdk = makeSdk(m, ...);
         * ```
         */
        template<class Archive>
        void serializeTo(Archive &ar) const {
            ar << m_sdk.get();
        }

    private:
        void syncForever(std::optional<int> retryTime = std::nullopt) const;

        lager::reader<SdkModel> m_sdk;
        lager::reader<ClientModel> m_client;
        ContextT m_ctx;
        std::optional<DepsT> m_deps;
    };

}
