/*
 * Copyright (C) 2021 Tusooa Zhu <tusooa@vista.aero>
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
#include <string>
#include <immer/map.hpp>
#include <immer/flex_vector.hpp>

#include <crypto.hpp>
#include <csapi/keys.hpp>

#include "cursorutil.hpp"

namespace Kazv
{
    struct DeviceKeyInfo
    {
        std::string deviceId;
        std::string ed25519Key;
        std::string curve25519Key;
        std::optional<std::string> displayName;
    };

    struct DeviceListTracker
    {
        immer::map<std::string /* userId */, bool /* outdated */> usersToTrackDeviceLists;
        immer::map<std::string /* userId */, immer::map<std::string /* deviceId */, DeviceKeyInfo>> deviceLists;

        template<class RangeT>
        void track(RangeT &&userIds) {
            for (auto userId : std::forward<RangeT>(userIds)) {
                usersToTrackDeviceLists = std::move(usersToTrackDeviceLists)
                    .set(userId, true);
            }
        }

        template<class RangeT>
        void untrack(RangeT &&userIds) {
            for (auto userId : std::forward<RangeT>(userIds)) {
                usersToTrackDeviceLists = std::move(usersToTrackDeviceLists).erase(userId);
            }
        }

        immer::flex_vector<std::string> outdatedUsers() const;

        bool addDevice(std::string userId, std::string deviceId, QueryKeysJob::DeviceInformation deviceInfo, Crypto &crypto);

        void markUpToDate(std::string userId);

        std::optional<DeviceKeyInfo> get(std::string userId, std::string deviceId) const;

        std::optional<DeviceKeyInfo> findByEd25519Key(std::string userId, std::string ed25519Key) const;
        std::optional<DeviceKeyInfo> findByCurve25519Key(std::string userId, std::string curve25519Key) const;
    };
}
