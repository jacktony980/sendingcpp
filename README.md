
# libkazv

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
- E2EE (send and receive events only)

These functionalities are currently not supported:

- E2EE (for attachments)
- Banning and kicking
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

## Dependencies

libkazv depends on [lager](https://github.com/arximboldi/lager),
[immer](https://github.com/arximboldi/immer),
[zug](https://github.com/arximboldi/zug),
[boost](https://boost.org),
[nlohmann_json](https://github.com/nlohmann/json),
[cereal](https://github.com/USCiLab/cereal).

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
