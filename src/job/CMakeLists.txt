
include(linklibsys)

set(libkazvjob_SRCS
  cprjobhandler.cpp
  )

add_library(kazvjob ${libkazvjob_SRCS})
add_library(libkazv::kazvjob ALIAS kazvjob)
target_link_libraries(kazvjob PUBLIC Threads::Threads kazvbase)
target_link_libraries_system(kazvjob PUBLIC
  cpr::cpr
  )
target_include_directories(kazvjob
  INTERFACE
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
  $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/kazv>
  )

install( TARGETS kazvjob LIBRARY )
