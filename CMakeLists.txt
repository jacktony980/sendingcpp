# Do not let the option()s override variables here
cmake_minimum_required(VERSION 3.13)

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
option(libkazv_BUILD_KAZVJOB "Build libkazvjob the async and networking library" ON)
option(libkazv_OUTPUT_LEVEL "Output level: Debug>=90, Info>=70, Quiet>=20, no output=1" 0)

if((libkazv_BUILD_TESTS OR libkazv_BUILD_EXAMPLES) AND NOT libkazv_BUILD_KAZVJOB)
  message(FATAL_ERROR
    "You asked kazvjob not to be built, but asked to build tests or examples. Tests and examples both depend on kazvjob. This is not possible.")
endif()

# Build shared libraries by default
if(NOT DEFINED BUILD_SHARED_LIBS)
  set(BUILD_SHARED_LIBS ON)
endif()

find_package(Boost REQUIRED)


include(FetchContent)
if(libkazv_BUILD_KAZVJOB)
  set(USE_SYSTEM_CURL ON)
  set(BUILD_CPR_TESTS OFF)
  FetchContent_Declare(cpr GIT_REPOSITORY https://github.com/whoshuu/cpr.git GIT_TAG c8d33915dbd88ad6c92b258869b03aba06587ff9) # the commit hash for 1.5.0
  FetchContent_MakeAvailable(cpr)

  set(THREADS_PREFER_PTHREAD_FLAG ON)
  find_package(Threads REQUIRED)
endif()

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

set(lager_BUILD_TESTS OFF)
set(lager_BUILD_EXAMPLES OFF)
set(lager_BUILD_DOCS OFF)
set(lager_EMBED_RESOURCES_PATH OFF)
FetchContent_Declare(lager GIT_REPOSITORY https://github.com/arximboldi/lager GIT_TAG 5beaaa757ad43313b9358c9e73e072cfc690653c)
FetchContent_MakeAvailable(lager)

set(JUST_INSTALL_CEREAL ON)
FetchContent_Declare(cereal GIT_REPOSITORY https://github.com/USCiLab/cereal GIT_TAG v1.3.0)
FetchContent_MakeAvailable(cereal)

if(libkazv_BUILD_EXAMPLES)
  find_package(PkgConfig REQUIRED)
  pkg_check_modules(LIBHTTPSERVER REQUIRED libhttpserver)
endif()

if(libkazv_OUTPUT_LEVEL)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DLIBKAZV_OUTPUT_LEVEL=${libkazv_OUTPUT_LEVEL}")
endif()

add_subdirectory(src)
