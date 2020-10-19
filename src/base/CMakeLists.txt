
include(linklibsys)

set(kazvbase_SRCS
  debug.cpp
  event.cpp
  basejob.cpp
  )

add_library(kazvbase ${kazvbase_SRCS})
add_library(libkazv::kazvbase ALIAS kazvbase)

target_link_libraries_system(kazvbase
  PUBLIC nlohmann_json::nlohmann_json
  immer
  zug
  lager
  cereal)
target_include_directories(kazvbase
  INTERFACE
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
  $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/kazv/base>
  )

install(TARGETS kazvbase LIBRARY)
