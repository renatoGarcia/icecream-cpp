if(POLICY CMP0167)
    cmake_policy(SET CMP0167 NEW)
endif()

find_package(Boost 1.71.0 REQUIRED)

enable_testing()

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if (
    CMAKE_CXX_STANDARD EQUAL 11
    OR CMAKE_CXX_STANDARD EQUAL 14
    OR CMAKE_CXX_STANDARD EQUAL 17
    OR CMAKE_CXX_STANDARD EQUAL 20
)
    add_executable(test_c++11 test_c++11.cpp test_2.cpp)
    add_test(NAME test_c++11 COMMAND test_c++11)

    add_executable(test_strings_c++11 test_strings_c++11.cpp)
    add_test(NAME test_strings_c++11 COMMAND test_strings_c++11)

    add_executable(test_config test_config.cpp)
    add_test(NAME test_config COMMAND test_config)
endif()

if (CMAKE_CXX_STANDARD EQUAL 17 OR CMAKE_CXX_STANDARD EQUAL 20)
    add_executable(test_c++17 test_c++17.cpp)
    add_test(NAME test_c++17 COMMAND test_c++17)

    add_executable(test_strings_c++17 test_strings_c++17.cpp)
    add_test(NAME test_strings_c++17 COMMAND test_strings_c++17)
endif()

if (CMAKE_CXX_STANDARD EQUAL 20)
    add_executable(test_strings_c++20 test_strings_c++20.cpp)
    add_test(NAME test_strings_c++20 COMMAND test_strings_c++20)
endif()

if (
    CMAKE_CXX_COMPILER_ID MATCHES "Clang"
    AND CMAKE_CXX_COMPILER_VERSION VERSION_GREATER_EQUAL "15"
)
    add_executable(test_clang test_clang.cpp)
    add_test(NAME test_clang COMMAND test_clang)
endif()

include_directories(${PROJECT_SOURCE_DIR})
include_directories(${Boost_INCLUDE_DIRS})
