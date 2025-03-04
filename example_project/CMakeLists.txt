cmake_minimum_required(VERSION 3.0)

project(Icecream-cpp-example)

find_package(icecream-cpp)

add_executable(hello hello.cpp)
include_directories(SYSTEM ${icecream-cpp_INCLUDE_DIRS})

install(TARGETS hello DESTINATION bin)
