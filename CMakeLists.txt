cmake_minimum_required(VERSION 3.8)

if(NOT DEFINED PROJECT_NAME)
  if(NOT DEFINED libkazv_INSTALL_HEADERS)
    set(libkazv_INSTALL_HEADERS ON)
  endif()
endif()

project(libkazv)

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake" ${CMAKE_MODULE_PATH})
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")

option(libkazv_BUILD_TESTS "Build tests" ON)
option(libkazv_BUILD_EXAMPLES "Build examples" ON)
option(libkazv_OUTPUT_LEVEL "Output level: Debug>=90, Info>=70, Quiet>=20, no output=1" 0)

# Build shared libraries by default
if(NOT DEFINED BUILD_SHARED_LIBS)
  set(BUILD_SHARED_LIBS ON)
endif()

find_package(Boost REQUIRED)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

include(FetchContent)
set(USE_SYSTEM_CURL ON)
set(BUILD_CPR_TESTS OFF)
FetchContent_Declare(cpr GIT_REPOSITORY https://github.com/whoshuu/cpr.git GIT_TAG c8d33915dbd88ad6c92b258869b03aba06587ff9) # the commit hash for 1.5.0
FetchContent_MakeAvailable(cpr)

if(libkazv_BUILD_TESTS)
  FetchContent_Declare(Catch2 GIT_REPOSITORY https://github.com/catchorg/Catch2 GIT_TAG v2.13.0)
  FetchContent_MakeAvailable(Catch2)
endif()

set(JSON_BuildTests OFF)
FetchContent_Declare(nlohmann_json GIT_REPOSITORY https://github.com/nlohmann/json GIT_TAG v3.9.1)
FetchContent_MakeAvailable(nlohmann_json)

set(immer_BUILD_TESTS OFF)
set(immer_BUILD_EXAMPLES OFF)
set(immer_BUILD_DOCS OFF)
set(immer_BUILD_EXTRAS OFF)
FetchContent_Declare(immer GIT_REPOSITORY https://github.com/arximboldi/immer GIT_TAG 2076affd9d814afc019ba8cd8c2b18a6c79c9589)
FetchContent_MakeAvailable(immer)

set(zug_BUILD_TESTS OFF)
set(zug_BUILD_EXAMPLES OFF)
set(zug_BUILD_DOCS OFF)
FetchContent_Declare(zug GIT_REPOSITORY https://github.com/arximboldi/zug GIT_TAG 0bf540906165143eb8b195284c588be29438a16b)
FetchContent_MakeAvailable(zug)

# Do not let the option()s override variables here
cmake_policy(SET CMP0077 NEW)
set(lager_BUILD_TESTS OFF)
set(lager_BUILD_EXAMPLES OFF)
set(lager_BUILD_DOCS OFF)
set(lager_EMBED_RESOURCES_PATH OFF)
FetchContent_Declare(lager GIT_REPOSITORY https://github.com/arximboldi/lager GIT_TAG a112eed88789bb2986b7924b5b1e71c8a410f81a)
FetchContent_MakeAvailable(lager)


if(libkazv_OUTPUT_LEVEL)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DLIBKAZV_OUTPUT_LEVEL=${libkazv_OUTPUT_LEVEL}")
endif()

add_subdirectory(src)
