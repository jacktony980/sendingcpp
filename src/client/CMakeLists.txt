set(kazvclient_SRCS
  client.cpp
  actions/auth.cpp
  actions/sync.cpp
  actions/paginate.cpp
  actions/membership.cpp
  actions/states.cpp
  actions/send.cpp
  actions/sync.cpp
  room/room.cpp
  )

add_library(kazvclient ${kazvclient_SRCS})
add_library(libkazv::kazvclient ALIAS kazvclient)

target_link_libraries(kazvclient PUBLIC kazvbase kazvapi)

target_include_directories(kazvclient PRIVATE .)

target_include_directories(kazvclient
  INTERFACE
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
  $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/kazv/client>
  )

install(TARGETS kazvclient LIBRARY)
