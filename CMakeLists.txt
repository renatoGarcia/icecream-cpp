cmake_minimum_required(VERSION 3.16)

project(Icecream-test)

find_package(Boost 1.71.0 REQUIRED)

enable_testing()

if (NOT DEFINED ENV{CPP_STANDARD})
    message(FATAL_ERROR "A CPP_STANDARD environment variable must be defined to one of: 11, 14, 17.")
endif()

if ($ENV{CPP_STANDARD} EQUAL 11)
    set(CMAKE_CXX_STANDARD 11)
    add_compile_definitions(CPP_11)
elseif ($ENV{CPP_STANDARD} EQUAL 14)
    set(CMAKE_CXX_STANDARD 14)
    add_compile_definitions(CPP_14)
elseif ($ENV{CPP_STANDARD} EQUAL 17)
    set(CMAKE_CXX_STANDARD 17)
    add_compile_definitions(CPP_17)
else()
    message(FATAL_ERROR "invalid C++ standard")
endif()

set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include_directories(${Boost_INCLUDE_DIRS})

add_executable(test_exec test.cpp test_2.cpp)
add_test(NAME test COMMAND test_exec)
