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


#include "libkazv-config.hpp"

#include <fstream>

#include "file-desc.hpp"
#include "debug.hpp"

namespace Kazv
{
    FileProvider FileDesc::provider(const FileInterface &fh) const
    {
        if (m_inMemory) {
            return FileProvider(DumbFileProvider(m_content));
        }
        return fh.getProviderFor(*this);
    }

    DumbFileProvider DumbFileInterface::getProviderFor(FileDesc desc) const
    {
        using CharT = char;
        auto nameOpt = desc.name();
        kzo.base.dbg() << "DumbFileInterface::getProviderFor()" << std::endl;
        if (! nameOpt) { // should not happen
            kzo.base.dbg() << "No name provided, skip" << std::endl;
            return DumbFileProvider{FileContent{}};
        }

        auto stream = std::ifstream(nameOpt.value(), std::ios_base::binary);
        if (! stream) {
            kzo.base.dbg() << "Cannot open file, skip" << std::endl;
            return DumbFileProvider{FileContent{}};
        }
        auto content = FileContent(std::istreambuf_iterator<CharT>(stream),
                                   std::istreambuf_iterator<CharT>());
        return DumbFileProvider{std::move(content)};
    }

    bool FileDesc::operator==(const FileDesc &that) const
    {
        return m_inMemory == that.m_inMemory
            && m_content == that.m_content
            && m_name == that.m_name
            && m_contentType == that.m_contentType;
    }
}
