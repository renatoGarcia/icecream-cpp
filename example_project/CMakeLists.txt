cmake_minimum_required(VERSION 3.0)

project(IcecreamCpp-example)

find_package(IcecreamCpp)

add_executable(hello hello.cpp)
include_directories(SYSTEM ${IcecreamCpp_INCLUDE_DIRS})

install(TARGETS hello DESTINATION bin)
