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

#include "device-list-tracker.hpp"

#include <algorithm>

#include <zug/transducer/filter.hpp>

#include <debug.hpp>

namespace Kazv
{
    immer::flex_vector<std::string> DeviceListTracker::outdatedUsers() const
    {
        return intoImmer(
            immer::flex_vector<std::string>{},
            zug::filter([](auto n) {
                            auto [userId, outdated] = n;
                            return outdated;
                        })
            | zug::map([](auto n) {
                           auto [userId, outdated] = n;
                           return userId;
                       }),
            usersToTrackDeviceLists);
    }


    bool DeviceListTracker::addDevice(std::string userId, std::string deviceId, QueryKeysJob::DeviceInformation deviceInfo, Crypto &crypto)
    {
        using namespace CryptoConstants;
        if (userId != deviceInfo.userId
            || deviceId != deviceInfo.deviceId) {
            return false;
        }

        // if the ed25519 key changed, reject
        auto curEd25519Key = deviceInfo.keys[ed25519 + ":" + deviceId];
        if (deviceLists[userId].find(deviceId)
            && curEd25519Key != deviceLists[userId][deviceId].ed25519Key) {
            return false;
        }

        kzo.client.dbg() << "verifying device info" << std::endl;

        if (crypto.verify(deviceInfo, userId, deviceId, curEd25519Key)) {
            kzo.client.dbg() << "passed verification" << std::endl;
            auto info = DeviceKeyInfo{
                deviceId,
                deviceInfo.keys[ed25519 + ":" + deviceId],
                deviceInfo.keys[curve25519 + ":" + deviceId],
                deviceInfo.unsignedData ? deviceInfo.unsignedData.value().deviceDisplayName : std::nullopt
            };

            deviceLists = std::move(deviceLists)
                .update(userId, [=](auto deviceMap) {
                                    return std::move(deviceMap).set(deviceId, info);
                                });
            return true;
        }

        kzo.client.dbg() << "did not pass verification" << std::endl;
        return false;
    }

    void DeviceListTracker::markUpToDate(std::string userId)
    {
        usersToTrackDeviceLists = std::move(usersToTrackDeviceLists).set(userId, false);
    }

    std::optional<DeviceKeyInfo> DeviceListTracker::get(std::string userId, std::string deviceId) const
    {
        try {
            return deviceLists.at(userId).at(deviceId);
        } catch (const std::exception &) {
            return std::nullopt;
        }
    }

    std::optional<DeviceKeyInfo> DeviceListTracker::findByEd25519Key(
        std::string userId, std::string ed25519Key) const
    {
        auto devices = deviceLists.at(userId);

        auto it = std::find_if(devices.begin(), devices.end(),
                               [=](auto n) {
                                   auto [deviceId, info] = n;
                                   return info.ed25519Key == ed25519Key;
                               });
        if (it != devices.end()) {
            return it->second;
        } else {
            return std::nullopt;
        }
    }

    std::optional<DeviceKeyInfo> DeviceListTracker::findByCurve25519Key(
        std::string userId, std::string curve25519Key) const
    {
        auto devices = deviceLists.at(userId);

        auto it = std::find_if(devices.begin(), devices.end(),
                               [=](auto n) {
                                   auto [deviceId, info] = n;
                                   return info.curve25519Key == curve25519Key;
                               });
        if (it != devices.end()) {
            return it->second;
        } else {
            return std::nullopt;
        }
    }

}
