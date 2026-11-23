cmake_minimum_required(VERSION 3.15)

project(Icecream-cpp-installed-consumer LANGUAGES CXX)

# Seam 3: consume an installed Icecream-cpp (found through CMAKE_PREFIX_PATH).
find_package(icecream-cpp REQUIRED)

add_executable(consumer ../main.cpp)
target_link_libraries(consumer PRIVATE icecream-cpp::icecream-cpp)
