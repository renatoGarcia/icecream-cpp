cmake_minimum_required(VERSION 3.15)

project(Icecream-cpp-cpm-consumer LANGUAGES CXX)

# Reproduces issue #103 ("Not installable through CPM"). The parent project has testing
# enabled (BUILD_TESTING=ON) and no Boost available, and adds Icecream-cpp with the same
# CPMAddPackage call used by the reporter.
#
# CPM_icecream-cpp_SOURCE (set by the caller) makes CPM use the local working tree instead
# of downloading the GitHub release, so the test exercises the code being developed.
include(CTest)

include(cmake/CPM.cmake)

CPMAddPackage(
  NAME icecream-cpp
  VERSION "1.1.0"
  GITHUB_REPOSITORY "renatoGarcia/icecream-cpp"
  SYSTEM YES
)

# Seam 2: besides being added, the library must be usable through its public target.
option(CONSUMER_LINK_TARGET "Build an executable linking icecream-cpp::icecream-cpp" OFF)
if(CONSUMER_LINK_TARGET)
  add_executable(consumer ../main.cpp)
  target_link_libraries(consumer PRIVATE icecream-cpp::icecream-cpp)
endif()
