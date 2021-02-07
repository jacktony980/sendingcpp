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

#include <libkazv-config.hpp>

#include <boost/algorithm/string.hpp>

#include "status-utils.hpp"

#include "content.hpp"

namespace Kazv
{
    std::pair<std::string, std::string> mxcUriToMediaDesc(std::string mxcUri)
    {
        using namespace boost::algorithm;

        if (mxcUri.find("mxc://") != 0) {
            return {"", ""};
        }

        mxcUri.erase(0, 6); // remove "mxc://"

        std::vector<std::string> a;
        split(a, mxcUri, is_any_of("/"));
        if (a.size() != 2) {
            return {"", ""};
        }
        return { a[0], a[1] };
    }

    ClientResult updateClient(ClientModel m, UploadContentAction a)
    {
        auto job = m.job<UploadContentJob>()
            .make(a.content, a.filename, a.contentType)
            .withData(json{ {"uploadId", a.uploadId} });
        m.addJob(std::move(job));

        return { std::move(m), lager::noop };
    }

    ClientResult processResponse(ClientModel m, UploadContentResponse r)
    {
        auto uploadId = r.dataStr("uploadId");
        if (! r.success()) {
            m.addTrigger(UploadContentFailed{uploadId, r.errorCode(), r.errorMessage()});
            return { std::move(m), simpleFail };
        }
        auto mxcUri = r.contentUri();
        m.addTrigger(UploadContentSuccessful{mxcUri, uploadId});
        return {
            std::move(m),
            [=](auto &&) {
                return EffectStatus{/* succ = */ true, json{{"mxcUri", mxcUri}}};
            }
        };
    }

    ClientResult updateClient(ClientModel m, DownloadContentAction a)
    {
        auto [serverName, mediaId] = mxcUriToMediaDesc(a.mxcUri);
        auto job = m.job<GetContentJob>()
            .make(serverName, mediaId)
            .withData(json{ {"mxcUri", a.mxcUri} });
        m.addJob(std::move(job));

        return { std::move(m), lager::noop };
    }

    ClientResult processResponse(ClientModel m, GetContentResponse r)
    {
        auto mxcUri = r.dataStr("mxcUri");
        if (! r.success()) {
            m.addTrigger(DownloadContentFailed{mxcUri, r.errorCode(), r.errorMessage()});
            return { std::move(m), lager::noop };
        }

        m.addTrigger(DownloadContentSuccessful{mxcUri, r.data(), r.contentDisposition(), r.contentType()});
        return { std::move(m), lager::noop };
    }

    ClientResult updateClient(ClientModel m, DownloadThumbnailAction a)
    {
        auto [serverName, mediaId] = mxcUriToMediaDesc(a.mxcUri);
        std::optional<std::string> method;
        if (a.method) {
            method = a.method.value() == Crop ? "crop" : "scale";
        }
        auto job = m.job<GetContentThumbnailJob>()
            .make(serverName, mediaId, a.width, a.height, method)
            .withData(json{ {"mxcUri", a.mxcUri} });
        m.addJob(std::move(job));

        return { std::move(m), lager::noop };
    }

    ClientResult processResponse(ClientModel m, GetContentThumbnailResponse r)
    {
        auto mxcUri = r.dataStr("mxcUri");
        if (! r.success()) {
            m.addTrigger(DownloadThumbnailFailed{mxcUri, r.errorCode(), r.errorMessage()});
            return { std::move(m), lager::noop };
        }

        m.addTrigger(DownloadThumbnailSuccessful{mxcUri, r.data(), r.contentType()});
        return { std::move(m), lager::noop };
    }

}
