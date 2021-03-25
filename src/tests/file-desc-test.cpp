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

#include <catch2/catch.hpp>

#include <boost/asio.hpp>

#include <vector>
#include <cstdio>
#include <fstream>
#include <filesystem>

#include "kazvtest-respath.hpp"
#include "file-guard.hpp"

#include <file-desc.hpp>
#include <asio-std-file-handler.hpp>

using namespace Kazv;

using ByteArrT = immer::flex_vector<char>;

TEST_CASE("FileDesc with DumbFileProvider should work properly", "[base][file-desc]")
{

    auto desc = FileDesc(
        immer::flex_vector<char>{'f', 'o', 'o', 'b', 'a', 'r'});

    auto fh = FileInterface(DumbFileInterface{});

    auto stream = desc.provider(fh).getStream();

    stream.read(5, [](auto retCode, auto data) {
                       REQUIRE(retCode == FileOpRetCode::Success);
                       REQUIRE(data == ByteArrT{'f', 'o', 'o', 'b', 'a'});
                   });

    stream.write(ByteArrT{'w', 'o', 'w'},
                 [](auto retCode, auto size) {
                     REQUIRE(retCode == FileOpRetCode::Success);
                     REQUIRE(size == 3);
                 });

    stream.read(5, [](auto retCode, auto data) {
                       REQUIRE(retCode == FileOpRetCode::Success);
                       REQUIRE(data == ByteArrT{'r', 'w', 'o', 'w'});
                   });

    stream.read(5, [](auto retCode, auto data) {
                       REQUIRE(retCode == FileOpRetCode::Eof);
                       REQUIRE(data == ByteArrT{});
                   });

}

TEST_CASE("DumbFileInterface should work properly", "[base][file-desc]")
{
    auto name1 = std::filesystem::path(resPath) / "file-desc-test-res1";

    auto desc = FileDesc(name1.native());

    auto fh = FileInterface{DumbFileInterface{}};
    auto provider = desc.provider(fh);

    auto stream = provider.getStream();

    stream.read(5, [](auto retCode, auto data) {
                       REQUIRE(retCode == FileOpRetCode::Success);
                       REQUIRE(data == ByteArrT{'f', 'o', 'o', 'b', 'a'});
                   });

    stream.read(5, [](auto retCode, auto data) {
                       REQUIRE(retCode == FileOpRetCode::Success);
                       REQUIRE(data == ByteArrT{'r'});
                   });

    stream.read(5, [](auto retCode, auto data) {
                       REQUIRE(retCode == FileOpRetCode::Eof);
                       REQUIRE(data == ByteArrT{});
                   });

}

TEST_CASE("AsioStdFileHandler should work properly with reads", "[base][file-desc]")
{
    auto name1 = std::filesystem::path(resPath) / "file-desc-test-res1";

    auto desc = FileDesc(name1.native());

    auto io = boost::asio::io_context();

    auto fh = FileInterface{AsioStdFileHandler{io.get_executor()}};
    auto provider = desc.provider(fh);

    auto stream = provider.getStream();

    stream.read(5, [](auto retCode, auto data) {
                       REQUIRE(retCode == FileOpRetCode::Success);
                       REQUIRE(data == ByteArrT{'f', 'o', 'o', 'b', 'a'});
                   });

    stream.read(5, [](auto retCode, auto data) {
                       REQUIRE(retCode == FileOpRetCode::Success);
                       REQUIRE(data == ByteArrT{'r'});
                   });

    stream.read(5, [](auto retCode, auto data) {
                       REQUIRE(retCode == FileOpRetCode::Eof);
                       REQUIRE(data == ByteArrT{});
                   });

    io.run();
}

TEST_CASE("AsioStdFileHandler should work properly with writes", "[base][file-desc]")
{
    auto name1 = std::filesystem::path(resPath) / "file-desc-test-tmp1";

    auto guard = FileGuard{name1};

    auto desc = FileDesc(name1.native());

    auto io = boost::asio::io_context();

    auto fh = FileInterface{AsioStdFileHandler{io.get_executor()}};
    auto provider = desc.provider(fh);

    {
        auto stream = provider.getStream(FileOpenMode::Write);

        stream.write(ByteArrT{'f', 'o', 'o'},
                     [](auto retCode, auto count) {
                         REQUIRE(retCode == FileOpRetCode::Success);
                         REQUIRE(count == 3);
                     });

        stream.write(ByteArrT{'b', 'a', 'r'},
                     [](auto retCode, auto count) {
                         REQUIRE(retCode == FileOpRetCode::Success);
                         REQUIRE(count == 3);
                     });
    }

    io.run(); // wait for the write to finish, and destroy the stream.

    auto stream = provider.getStream(FileOpenMode::Read);
    stream.read(5, [](auto retCode, auto data) {
                       REQUIRE(retCode == FileOpRetCode::Success);
                       REQUIRE(data == ByteArrT{'f', 'o', 'o', 'b', 'a'});
                   });

    stream.read(5, [](auto retCode, auto data) {
                       REQUIRE(retCode == FileOpRetCode::Success);
                       REQUIRE(data == ByteArrT{'r'});
                   });

    stream.read(5, [](auto retCode, auto data) {
                       REQUIRE(retCode == FileOpRetCode::Eof);
                       REQUIRE(data == ByteArrT{});
                   });

    io.run();
}


TEST_CASE("AsioStdFileHandler should work properly with binary data", "[base][file-desc]")
{
    auto name1 = std::filesystem::path(resPath) / "file-desc-test-tmp2";

    auto guard = FileGuard{name1};

    auto desc = FileDesc(name1.native());

    auto io = boost::asio::io_context();

    auto fh = FileInterface{AsioStdFileHandler{io.get_executor()}};
    auto provider = desc.provider(fh);

    {
        auto stream = provider.getStream(FileOpenMode::Write);

        stream.write(ByteArrT{'\0', '\r', '\n', '\n', '\r'},
                     [](auto retCode, auto count) {
                         REQUIRE(retCode == FileOpRetCode::Success);
                         REQUIRE(count == 5);
                     });
    }

    io.run(); // wait for the write to finish, and destroy the stream.

    auto stream = provider.getStream(FileOpenMode::Read);
    stream.read(5, [](auto retCode, auto data) {
                       REQUIRE(retCode == FileOpRetCode::Success);
                       REQUIRE(data == ByteArrT{'\0', '\r', '\n', '\n', '\r'});
                   });

    stream.read(5, [](auto retCode, auto data) {
                       REQUIRE(retCode == FileOpRetCode::Eof);
                       REQUIRE(data == ByteArrT{});
                   });

    io.run();
}
