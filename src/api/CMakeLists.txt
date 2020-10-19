
include(linklibsys)

set(FULL_CSAPI_DIR csapi)
file(GLOB_RECURSE kazvapi_SRCS ${FULL_CSAPI_DIR}/*.cpp)

add_library(kazvapi ${kazvapi_SRCS})
add_library(libkazv::kazvapi ALIAS kazvapi)

target_link_libraries(kazvapi PUBLIC kazvbase)

target_include_directories(kazvapi PRIVATE .)

target_include_directories(kazvapi
  INTERFACE
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
  $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/kazv/api>
  )

install(TARGETS kazvapi LIBRARY)
