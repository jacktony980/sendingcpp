

include(CTest)

set(KAZVTEST_RESPATH ${CMAKE_CURRENT_SOURCE_DIR}/resources)
configure_file(kazvtest-respath.hpp.in kazvtest-respath.hpp)

add_executable(kazvtest
  testmain.cpp
  basejobtest.cpp
  cursorutiltest.cpp
  client/client-test-util.cpp
  client/sync-test.cpp
  client/content-test.cpp
  client/paginate-test.cpp
  kazvjobtest.cpp
  event-emitter-test.cpp
  crypto-test.cpp
  promise-test.cpp
  store-test.cpp
  file-desc-test.cpp
  )

target_include_directories(
  kazvtest
  PRIVATE ${CMAKE_CURRENT_BINARY_DIR})

target_link_libraries(kazvtest
  PRIVATE Catch2::Catch2
  PRIVATE kazv
  PRIVATE kazveventemitter
  PRIVATE kazvjob
  PRIVATE nlohmann_json::nlohmann_json
  PRIVATE immer
  PRIVATE lager
  PRIVATE zug)
