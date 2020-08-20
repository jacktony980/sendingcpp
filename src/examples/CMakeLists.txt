
add_executable(basicexample
  basic/main.cpp)

target_link_libraries(basicexample
  PRIVATE kazv
  PRIVATE immer
  PRIVATE zug
  PRIVATE lager)
