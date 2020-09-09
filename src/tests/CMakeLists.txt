

include(CTest)

add_executable(kazvtest
  testmain.cpp
  basejobtest.cpp
  cursorutiltest.cpp
  )

target_link_libraries(kazvtest
  PRIVATE Catch2::Catch2
  PRIVATE kazv
  PRIVATE kazvjob
  PRIVATE nlohmann_json::nlohmann_json
  PRIVATE immer
  PRIVATE lager
  PRIVATE zug)
