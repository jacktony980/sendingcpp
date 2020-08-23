
include_directories(job . csapi)
set(FULL_CSAPI_DIR csapi)
file(GLOB_RECURSE api_SRCS ${FULL_CSAPI_DIR}/*.cpp)
set(libkazv_SRCS
  #  api/wellknown.cpp
  ${api_SRCS}
  debug.cpp
  job/basejob.cpp
  job/cprjobhandler.cpp
  client/client.cpp
  )

add_library(kazv ${libkazv_SRCS})
add_library(libkazv::kazv ALIAS kazv)

target_link_libraries(kazv
  PUBLIC cpr::cpr
  PUBLIC nlohmann_json::nlohmann_json
  PUBLIC immer
  PUBLIC zug
  PUBLIC lager)
target_include_directories(kazv
  INTERFACE
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
  $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/kazv>
  )

install(
  TARGETS kazv

  LIBRARY
  )

if(libkazv_INSTALL_HEADERS)
  install(
    DIRECTORY
    ${CMAKE_CURRENT_SOURCE_DIR}/

    DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/kazv"

    FILES_MATCHING PATTERN "*.hpp"

    PATTERN "tests" EXCLUDE
    )
endif()

# export(TARGETS kazv)

if(libkazv_BUILD_TESTS)
  add_subdirectory(tests)
endif()

if(libkazv_BUILD_EXAMPLES)
  add_subdirectory(examples)
endif()
