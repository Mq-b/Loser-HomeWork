cmake_minimum_required(VERSION 3.17)
project(encodingcheck LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 20)

add_subdirectory(xdgmime)
add_subdirectory(is_utf8)

add_executable(encodingcheck encodingcheck.cpp)
target_link_libraries(encodingcheck PRIVATE libxdgmime is_utf8)
