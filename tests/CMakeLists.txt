if(POLICY CMP0167)
  cmake_policy(SET CMP0167 NEW)
endif()

option(ENABLE_RANGE_V3 "If ON the tests to Range-v3 library will be enabled" OFF)
option(ENABLE_FMT "If ON the tests to {fmt} library will be enabled" OFF)

find_package(Catch2 REQUIRED)
find_package(Boost REQUIRED)

if (ENABLE_RANGE_V3)
  find_package(range-v3 REQUIRED)
endif()

if (ENABLE_FMT)
  find_package(fmt REQUIRED)
endif()

enable_testing()

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include_directories(${PROJECT_SOURCE_DIR})

function(declare_test name)
  add_executable(test_${name} test_${name}.cpp)
  add_test(NAME test_${name} COMMAND test_${name})
  target_link_libraries(test_${name} PRIVATE Catch2::Catch2 Boost::headers)

  if (ENABLE_RANGE_V3)
    target_link_libraries(test_${name} PRIVATE range-v3::range-v3)
  endif()

  if (ENABLE_FMT)
    target_link_libraries(test_${name} PRIVATE fmt::fmt)
  endif()
endfunction()

if (CMAKE_CXX_STANDARD GREATER_EQUAL 11)
  declare_test(c++11)
  declare_test(strings_c++11)
  declare_test(config)
  declare_test(slicing)
  if (ENABLE_RANGE_V3)
    declare_test(range_v3)
  endif()
  if (ENABLE_FMT)
    declare_test(fmt)
  endif()
endif()

if (CMAKE_CXX_STANDARD GREATER_EQUAL 17)
  declare_test(c++17)
  declare_test(strings_c++17)
endif()

if (CMAKE_CXX_STANDARD GREATER_EQUAL 20)
  declare_test(c++20)
  declare_test(strings_c++20)
endif()

if (CMAKE_CXX_STANDARD GREATER_EQUAL 23)
  declare_test(c++23)
endif()

if (
  CMAKE_CXX_COMPILER_ID MATCHES "Clang"
  AND CMAKE_CXX_COMPILER_VERSION VERSION_GREATER_EQUAL "15"
)
  declare_test(clang)
endif()
