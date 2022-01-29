/*
 * This file is part of libkazv.
 * SPDX-FileCopyrightText: 2020 Tusooa Zhu <tusooa@kazv.moe>
 * SPDX-License-Identifier: AGPL-3.0-or-later
 */

#include <libkazv-config.hpp>

#include "client-test-util.hpp"

ClientModel createTestClientModel()
{
    ClientModel m;
    m.serverUrl = "example.com";
    m.userId = "@bob:example.com";
    m.token = "exampletoken";
    m.deviceId = "exampledevice";
    m.loggedIn = true;
    return m;
}
