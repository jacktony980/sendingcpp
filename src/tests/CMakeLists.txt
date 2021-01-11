

include(CTest)

add_executable(kazvtest
  testmain.cpp
  basejobtest.cpp
  cursorutiltest.cpp
  client/client-test-util.cpp
  client/sync-test.cpp
  kazvjobtest.cpp
  event-emitter-test.cpp
  )

target_link_libraries(kazvtest
  PRIVATE Catch2::Catch2
  PRIVATE kazv
  PRIVATE kazveventemitter
  PRIVATE kazvjob
  PRIVATE nlohmann_json::nlohmann_json
  PRIVATE immer
  PRIVATE lager
  PRIVATE zug)
