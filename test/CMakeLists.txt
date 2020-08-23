find_package(Boost 1.71.0 REQUIRED)

enable_testing()

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(CMAKE_CXX_STANDARD EQUAL 11)
    add_compile_definitions(CPP_11)
elseif(CMAKE_CXX_STANDARD EQUAL 14)
    add_compile_definitions(CPP_14)
elseif(CMAKE_CXX_STANDARD EQUAL 17)
    add_compile_definitions(CPP_17)
endif()

include_directories(${PROJECT_SOURCE_DIR})
include_directories(${Boost_INCLUDE_DIRS})

add_executable(test_exec test.cpp test_2.cpp)
add_test(NAME test COMMAND test_exec)
