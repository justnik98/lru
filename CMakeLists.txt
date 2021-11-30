cmake_minimum_required(VERSION 3.20)
project(lru)

set(CMAKE_CXX_STANDARD 20)
include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

add_executable(lru main.cpp src/lru.hpp)
target_link_libraries(lru PUBLIC ${CONAN_LIBS})