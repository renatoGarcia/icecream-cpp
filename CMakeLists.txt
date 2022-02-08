# DEV_HASH: $Format:%H$

cmake_minimum_required(VERSION 3.16)
cmake_policy(SET CMP0048 NEW)

project(IcecreamCpp VERSION 0.3.1 LANGUAGES CXX)

include(CMakePackageConfigHelpers)

option(BUILD_TESTING "" OFF)
include(CTest)

if(UNIX)
  include(GNUInstallDirs)
  set(DEF_INSTALL_CMAKE_DIR "${CMAKE_INSTALL_LIBDIR}/cmake/IcecreamCpp")
  set(DEF_INSTALL_INCLUDE_DIR "${CMAKE_INSTALL_INCLUDEDIR}")
elseif(WIN32 AND NOT CYGWIN)
  set(DEF_INSTALL_CMAKE_DIR "CMake")
  set(DEF_INSTALL_INCLUDE_DIR "include")
else()
  set(INSTALL_DIR "" CACHE PATH)
  set(DEF_INSTALL_CMAKE_DIR "${INSTALL_DIR}/cmake/IcecreamCpp")
  set(DEF_INSTALL_INCLUDE_DIR "${INSTALL_DIR}/include")
endif()

configure_package_config_file(IcecreamCppConfig.cmake.in
  "${PROJECT_BINARY_DIR}/IcecreamCppConfig.cmake"
  INSTALL_DESTINATION ${DEF_INSTALL_CMAKE_DIR}
  PATH_VARS DEF_INSTALL_INCLUDE_DIR
)
write_basic_package_version_file(
  "${PROJECT_BINARY_DIR}/IcecreamCppConfigVersion.cmake"
  COMPATIBILITY SameMajorVersion
  ARCH_INDEPENDENT
)

install(
  FILES
    "${PROJECT_BINARY_DIR}/IcecreamCppConfig.cmake"
    "${PROJECT_BINARY_DIR}/IcecreamCppConfigVersion.cmake"
  DESTINATION ${DEF_INSTALL_CMAKE_DIR}
)

install(
  FILES "icecream.hpp"
  DESTINATION ${DEF_INSTALL_INCLUDE_DIR}
)

if(BUILD_TESTING)
  add_subdirectory(test)
endif()
