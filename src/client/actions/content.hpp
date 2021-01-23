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

#include "csapi/content-repo.hpp"

namespace Kazv
{
    std::pair<std::string, std::string> mxcUriToMediaDesc(std::string mxcUri);

    ClientResult updateClient(ClientModel m, UploadContentAction a);
    ClientResult processResponse(ClientModel m, UploadContentResponse r);

    ClientResult updateClient(ClientModel m, DownloadContentAction a);
    ClientResult processResponse(ClientModel m, GetContentResponse r);

    ClientResult updateClient(ClientModel m, DownloadThumbnailAction a);
    ClientResult processResponse(ClientModel m, GetContentThumbnailResponse r);
}
