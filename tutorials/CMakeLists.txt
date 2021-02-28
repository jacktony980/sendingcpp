
cmake_minimum_required(VERSION 3.18)
project(libkazv-tutorials)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")

find_package(libkazv REQUIRED COMPONENTS job)

add_executable(tutorial0
  tutorial0.cpp)

target_link_libraries(tutorial0 libkazv::kazvall libkazv::kazvjob)
