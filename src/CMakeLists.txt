
include(linklibsys)
include_directories(job . csapi)
set(FULL_CSAPI_DIR csapi)
file(GLOB_RECURSE api_SRCS ${FULL_CSAPI_DIR}/*.cpp)
set(libkazv_SRCS
  #  api/wellknown.cpp
  ${api_SRCS}
  debug.cpp
  event.cpp
  job/basejob.cpp
  client/client.cpp
  client/sync.cpp
  client/paginate.cpp
  client/roomeffects.cpp
  client/room/room.cpp
  )

add_library(kazv ${libkazv_SRCS})
add_library(libkazv::kazv ALIAS kazv)

target_link_libraries_system(kazv
  PUBLIC nlohmann_json::nlohmann_json
  immer
  zug
  lager
  cereal)
target_include_directories(kazv
  INTERFACE
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
  $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/kazv>
  )

install(
  TARGETS kazv

  LIBRARY
  )

set(libkazvjob_SRCS
  job/cprjobhandler.cpp
  )

if(libkazv_BUILD_KAZVJOB)
  add_library(kazvjob ${libkazvjob_SRCS})
  add_library(libkazv::kazvjob ALIAS kazvjob)
  target_link_libraries(kazvjob PUBLIC Threads::Threads)
  target_link_libraries_system(kazvjob PUBLIC
    cpr::cpr
    nlohmann_json::nlohmann_json
    immer
    zug
    lager
    cereal
    )
  target_include_directories(kazvjob
    INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/kazv>
    )

  install( TARGETS kazvjob LIBRARY )
endif()

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
