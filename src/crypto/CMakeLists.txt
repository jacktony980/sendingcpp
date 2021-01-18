set(kazvcrypto_SRCS
  crypto.cpp
  session.cpp
  inbound-group-session.cpp
  outbound-group-session.cpp
  )

add_library(kazvcrypto ${kazvcrypto_SRCS})
add_library(libkazv::kazvcrypto ALIAS kazvcrypto)
set_target_properties(kazvcrypto PROPERTIES VERSION ${libkazv_VERSION_STRING} SOVERSION 0)

target_link_libraries(kazvcrypto PUBLIC kazvbase Olm::Olm)

target_include_directories(kazvcrypto PRIVATE .)

target_include_directories(kazvcrypto
  INTERFACE
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
  $<INSTALL_INTERFACE:include/kazv/crypto>
  )

install(TARGETS kazvcrypto EXPORT libkazvConfig LIBRARY)
