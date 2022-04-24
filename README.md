# libkazv  {#mainpage}

[![pipeline status](https://lily.kazv.moe/kazv/libkazv/badges/servant/pipeline.svg)](https://lily.kazv.moe/kazv/libkazv/-/commits/servant)
[![coverage report](https://lily.kazv.moe/kazv/libkazv/badges/servant/coverage.svg)](https://lily.kazv.moe/kazv/libkazv/-/commits/servant)

libkazv is a matrix client sdk built upon [lager](https://github.com/arximboldi/lager)
and the value-oriented design it enables.

# Functionalities

libkazv support the following functionalities:

- Logging in
- Receiving room states
- Receiving room messages
- Receiving account data
- Receiving presence
- Sending messages
- Send room state events
- Create rooms
- Room invites
- Join rooms
- Typing notifications
- Receipts and fully-read markers
- Leaving and forgetting rooms
- Content repository
- Send-to-device messages
- E2EE (send and receive events only and attachments)
- Banning and kicking

These functionalities are currently not supported:

- Setting presence
- Device management
- Direct messages
- Redactions
- Room history visibility
- Registering
- VoIP
- Room tagging
- Searching
- Room previews
- Mentions

These functionalities may be implemented, but in a low priority:

- Push notifications
- Third-party invites
- Guest access
- Server administration
- Event context
- Ignoring users
- Reporting content
- Third party networks
- Server notices
- Moderation policy lists

libkazv is not planning to support these functionalities:

- Single Sign On


# Build and Use

## For Gentoo users

If you are using Gentoo, you can use [tusooa-overlay][tusooa-overlay]
to install libkazv. The dependency `olm` can be installed from
[src_prepare-overlay][src-prep].

[tusooa-overlay]: https://gitlab.com/tusooa/tusooa-overlay
[src-prep]: https://gitlab.com/src_prepare/src_prepare-overlay

## Dependencies

libkazv depends on [lager](https://github.com/arximboldi/lager),
[immer](https://github.com/arximboldi/immer),
[zug](https://github.com/arximboldi/zug),
[boost](https://boost.org),
[nlohmann_json](https://github.com/nlohmann/json),
[olm](https://gitlab.matrix.org/matrix-org/olm),
[libcrypto++](https://cryptopp.com/).

kazvjob also depends on [cpr](https://github.com/whoshuu/cpr).

Tests also depend on [Catch2](https://github.com/catchorg/Catch2).

Examples also depend on [libhttpserver](https://github.com/etr/libhttpserver).

All the dependencies mentioned, except for boost and libhttpserver,
are automatically fetched by cmake `FetchContent`.

## Process

You can build libkazv through the standard CMake process:

```
mkdir -pv build && cd build
cmake .. -DCMAKE_INSTALL_PREFIX=/path/to/prefix
make install
```

libkazv offers the following CMake options:

- `libkazv_BUILD_TESTS`: boolean value to specify whether to build tests
- `libkazv_BUILD_EXAMPLES`: boolean value to specify whether to build examples
- `libkazv_OUTPUT_LEVEL`: integral value from 0 to 100 to determine what kinds
  of logs are shown. Setting to 100 makes libkazv output the most debug
  information.
- `libkazv_INSTALL_HEADERS`: boolean value to determine whether to install
  libkazv's headers. This is by default set to OFF when libkazv is built
  as a subproject.

libkazv can be incorporated into your project using CMake `FetchContent()`.
It can also be used via `find_package(libkazv)`.
It has a few libraries you can link to:

- `libkazv::kazvall` is the one that contains API call definitions
  and client logic. It does not, however, define how the jobs are
  fetched.
- `libkazv::kazvjob` is a tiny library that provides async
  and network fetching functionalities. There is one class
  `CprJobHandler` that implements `JobInterface` in `kazv`.
  You can link your program to `kazvjob` or make up another
  job handler using what you choose as async and network
  libraries. To switch from one job handler to another,
  you only need to change one or two lines in your program.

  Note that you will need to add `COMPONENTS job` to the arguments
  of `find_package()` to use this.

# Tutorials

Tutorial 0: [Getting started with libkazv][tut0]

[tut0]: https://gitlab.com/kazv/libkazv/-/blob/servant/tutorials/tutorial0.md

# APIs

The API documentation is available at https://mxc.kazv.moe/libkazv/ .
You can also use `doxygen Doxyfile` at the root directory of the repository
to build docs locally. It will be generated in `doc/`.

You SHOULD use only the following APIs in your program:

1. `Kazv::makeSdk`, `Kazv::makeDefaultEncryptedSdk`, `Kazv::makeDefaultSdkWithCryptoRandom`.
2. Default constructor of `Kazv::SdkModel`.
3. Constructors of `Kazv::CprJobHandler`,
   `Kazv::AsioPromiseHandler`.
4. `Kazv::Sdk`, `Kazv::Client`, `Kazv::Room`, but not their constructors.
5. `Kazv::LagerStoreEventEmitter`.
6. The classes that are required to interact with (e.g. return type or argument of)
   the classes in 4 and 5 (e.g. `Kazv::Event`).

Anything in the `kazvapi` module (i.e. the namespace `Kazv::Api`) is not meant
to be used directly.

You should not rely on the return type of a function if it is declared as
`auto` (without trailing return type notation). If it returns `auto`, you
should also use `auto` to store that the return value.

If the documented return type is a `using` declaration in that class, you
should either use `auto` or the exact alias (not the aliased type) for that.
For example, in `Client`, a lot of functions return `Client::PromiseT`, so
you should use `auto` or `Client::PromiseT` to refer to the return type.

# Versioning

The following versioning strategies are used:

1. There are two versions in this library:
   - Package version, specified by `libkazv_VERSION_STRING` in CMakeLists.txt ,
     in the format of `X.Y.Z`. `X`, `Y`, `Z` are called Major, Minor, and Patch
     Versions respectively.
   - so version, specified by `libkazv_SOVERSION` in CMakeLists.txt .

2. When there is a new release, the versions are determined by the following rules:
   1. If there are no changes to any of the public headers (headers containing public
      APIs) existing in the last release,
      keep Major and Minor Versions as the same, increase Patch Version by 1,
      and keep so version as the same.
   2. If 2.1 does not hold, and all possible source code invoking only public APIs in
      the last release according to the guidelines (e.g. always use `auto` when specified)
      that compiles under a certain GCC version will still compile under that GCC version,
      keep Major Version
      as the same, increase Minor Version by 1, let Patch Version be 0, and increase
      so version by 1.
   3. Otherwise, increase Major Version by 1, let Minor and Patch Versions be 0, and
      increase so version by 1.

   A tldr but inaccurate version of the rules: If the new release is binary-compatible
   with the last one, bump Patch Version; if the new release is not binary-compatible but
   source-compatible with the last one, bump Minor Version and so version,
   reset Patch Version; otherwise, bump Major Version and so version, reset Minor and Patch
   Versions.

# Acknowledgement

libkazv uses [gtad](https://github.com/KitsuneRal/gtad) to generate the API
definitions it needed. The source of the Matrix API is
[https://github.com/matrix-org/matrix-doc](https://github.com/matrix-org/matrix-doc)
. The gtad configuration files and json/query serializing used in libkazv are
adapted from the ones in [libQuotient](https://github.com/quotient-im/libQuotient).
libQuotient is released under GNU LGPL v2.1. The changes in said files
in libkazv compared to libQuotient's are:

- Get rid of the `avoidCopy` and `moveOnly` markers
- Use data types from `immer` and `std` instead of Qt
- Use `nlohmann::json` instead of Qt's JSON library

# License

Copyright (C) 2020 Tusooa Zhu

libkazv is free software: you can redistribute it and/or modify
it under the terms of the GNU Affero General Public License as
published by the Free Software Foundation, either version 3 of the
License, or (at your option) any later version.

libkazv is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Affero General Public License for more details.

You should have received a copy of the GNU Affero General Public License
along with libkazv.  If not, see <https://www.gnu.org/licenses/>.
