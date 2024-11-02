if(POLICY CMP0167)
    cmake_policy(SET CMP0167 NEW)
endif()

find_package(Catch2 REQUIRED)
find_package(Boost REQUIRED)

enable_testing()

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include_directories(${PROJECT_SOURCE_DIR})

function(declare_test name)
    add_executable(test_${name} test_${name}.cpp)
    add_test(NAME test_${name} COMMAND test_${name})
    target_link_libraries(test_${name} PRIVATE Catch2::Catch2 Boost::headers)
endfunction()

if (
    CMAKE_CXX_STANDARD EQUAL 11
    OR CMAKE_CXX_STANDARD EQUAL 14
    OR CMAKE_CXX_STANDARD EQUAL 17
    OR CMAKE_CXX_STANDARD EQUAL 20
)
    declare_test(c++11)
    declare_test(strings_c++11)
    declare_test(config)
    declare_test(slicing)
endif()

if (CMAKE_CXX_STANDARD EQUAL 17 OR CMAKE_CXX_STANDARD EQUAL 20)
    declare_test(c++17)
    declare_test(strings_c++17)
endif()

if (CMAKE_CXX_STANDARD EQUAL 20)
    declare_test(c++20)
    declare_test(strings_c++20)
endif()

if (
    CMAKE_CXX_COMPILER_ID MATCHES "Clang"
    AND CMAKE_CXX_COMPILER_VERSION VERSION_GREATER_EQUAL "15"
)
    declare_test(clang)
endif()
