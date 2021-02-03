cmake_minimum_required(VERSION 3.17)

project(unittests)
add_subdirectory(lib)
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})
include_directories(includes)
add_executable(unit_tests_run lruTests.cpp)

target_link_libraries(unit_tests_run gtest gtest_main)