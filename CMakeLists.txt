# DEV_HASH: $Format:%H$

cmake_minimum_required(VERSION 3.16)
cmake_policy(SET CMP0048 NEW)

project(icecream-cpp VERSION 1.0.0 LANGUAGES CXX)

include(CMakePackageConfigHelpers)

option(BUILD_TESTING "" OFF)
include(CTest)

if(UNIX)
  include(GNUInstallDirs)
  set(DEF_INSTALL_CMAKE_DIR "${CMAKE_INSTALL_LIBDIR}/cmake/icecream-cpp")
  set(DEF_INSTALL_INCLUDE_DIR "${CMAKE_INSTALL_INCLUDEDIR}")
elseif(WIN32 AND NOT CYGWIN)
  set(DEF_INSTALL_CMAKE_DIR "CMake")
  set(DEF_INSTALL_INCLUDE_DIR "include")
else()
  set(INSTALL_DIR "" CACHE PATH)
  set(DEF_INSTALL_CMAKE_DIR "${INSTALL_DIR}/cmake/icecream-cpp")
  set(DEF_INSTALL_INCLUDE_DIR "${INSTALL_DIR}/include")
endif()

configure_package_config_file(icecream-cpp-config.cmake.in
  "${PROJECT_BINARY_DIR}/icecream-cpp-config.cmake"
  INSTALL_DESTINATION ${DEF_INSTALL_CMAKE_DIR}
  PATH_VARS DEF_INSTALL_INCLUDE_DIR
)
write_basic_package_version_file(
  "${PROJECT_BINARY_DIR}/icecream-cpp-config-version.cmake"
  COMPATIBILITY SameMajorVersion
  ARCH_INDEPENDENT
)

install(
  FILES
    "${PROJECT_BINARY_DIR}/icecream-cpp-config.cmake"
    "${PROJECT_BINARY_DIR}/icecream-cpp-config-version.cmake"
  DESTINATION ${DEF_INSTALL_CMAKE_DIR}
)

install(
  FILES "icecream.hpp"
  DESTINATION ${DEF_INSTALL_INCLUDE_DIR}
)

if(BUILD_TESTING)
  add_subdirectory(tests)
endif()
