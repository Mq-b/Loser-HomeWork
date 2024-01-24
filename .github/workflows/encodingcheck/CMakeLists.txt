cmake_minimum_required(VERSION 3.17)
project(encodingcheck LANGUAGES CXX)

add_subdirectory(xdgmime)

add_executable(encodingcheck encodingcheck.cpp)
target_link_libraries(encodingcheck PRIVATE libxdgmime)