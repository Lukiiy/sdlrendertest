cmake_minimum_required(VERSION 4.4)
project(test CXX)

set(CMAKE_CXX_STANDARD 23)

find_package(SDL3 REQUIRED)

add_executable(test main.cpp)
target_link_libraries(test PRIVATE SDL3::SDL3)