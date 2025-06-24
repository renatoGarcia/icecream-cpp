cmake_minimum_required(VERSION 3.15)

project(Icecream-cpp-example)

find_package(icecream-cpp REQUIRED)

add_executable(hello hello.cpp)
target_link_libraries(hello PRIVATE icecream-cpp::icecream-cpp)

install(TARGETS hello DESTINATION bin)
