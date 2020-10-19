
add_library(kazveventemitter INTERFACE)
add_library(libkazv::kazveventemitter ALIAS kazveventemitter)

target_link_libraries(kazveventemitter INTERFACE kazvbase)

target_include_directories(kazveventemitter
  INTERFACE
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
  $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/kazv>
  )
