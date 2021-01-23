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

#pragma once
#include <libkazv-config.hpp>

#include "client-model.hpp"

namespace Kazv
{
    struct SdkModel;

    struct AfterGetRoomStates
    {
        std::string roomId;
        Event event;
    };

    using SdkAction = std::variant<
        ClientAction,
        AfterGetRoomStates
        >;

    using SdkEffect = lager::effect<SdkAction, lager::deps<JobInterface &, EventInterface &>>;

    using SdkResult = std::pair<SdkModel, SdkEffect>;

    struct SdkModel
    {
        ClientModel client;

        inline operator ClientModel() const { return client; }

        using Action = SdkAction;
        using Effect = SdkEffect;
        using Result = SdkResult;

        static SdkResult update(SdkModel s, SdkAction a);
    };

    template<class Archive>
    void serialize(Archive &ar, SdkModel &s, std::uint32_t const /*version*/)
    {
        ar(s.client);
    }
}

CEREAL_CLASS_VERSION(Kazv::SdkModel, 0);
