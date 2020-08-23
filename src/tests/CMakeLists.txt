

include(CTest)

add_executable(basejobtest
  testallapi.cpp

  basejobtest.cpp
  )
target_link_libraries(basejobtest
  PRIVATE Catch2::Catch2
  PRIVATE kazv
  PRIVATE kazvjob
  PRIVATE nlohmann_json::nlohmann_json
  PRIVATE immer
  PRIVATE lager
  PRIVATE zug)
