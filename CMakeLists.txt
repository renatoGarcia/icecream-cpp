# DEV_HASH: $Format:%H$

cmake_minimum_required(VERSION 3.15)
cmake_policy(SET CMP0048 NEW)

project(icecream-cpp VERSION 1.0.0 LANGUAGES CXX)

include(CMakePackageConfigHelpers)
include(GNUInstallDirs)

option(ICECREAM_BUILD_TESTS "If ON the Icecream-cpp tests will be built" OFF)

add_library(icecream-cpp INTERFACE)
add_library(icecream-cpp::icecream-cpp ALIAS icecream-cpp)

target_include_directories(
  icecream-cpp
  INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

install(
  FILES "${CMAKE_CURRENT_SOURCE_DIR}/icecream.hpp"
  DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}"
)

install(
  TARGETS icecream-cpp
  EXPORT icecream-cpp-targets
)

install(
  EXPORT icecream-cpp-targets
  FILE icecream-cpp-targets.cmake
  NAMESPACE icecream-cpp::
  DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/icecream-cpp"
)

write_basic_package_version_file(
  "${CMAKE_CURRENT_BINARY_DIR}/icecream-cpp-config-version.cmake"
  COMPATIBILITY SameMajorVersion
  ARCH_INDEPENDENT
)

install(
  FILES
    "${CMAKE_CURRENT_SOURCE_DIR}/icecream-cpp-config.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/icecream-cpp-config-version.cmake"
  DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/icecream-cpp"
)

if(ICECREAM_BUILD_TESTS)
  enable_testing()
  add_subdirectory(tests)
endif()
