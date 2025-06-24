# DEV_HASH: $Format:%H$

cmake_minimum_required(VERSION 3.15)
cmake_policy(SET CMP0048 NEW)

project(icecream-cpp VERSION 1.0.0 LANGUAGES CXX)

include(CMakePackageConfigHelpers)
include(GNUInstallDirs)

option(BUILD_TESTING "If ON the tests will be enabled" OFF)
include(CTest)

add_library(icecream-cpp INTERFACE)

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

if(BUILD_TESTING)
  add_subdirectory(tests)
endif()
