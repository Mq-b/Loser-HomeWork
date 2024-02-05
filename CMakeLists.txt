cmake_minimum_required(VERSION 3.17)
project(loser_homework)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH}
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

option(BUILD_ENCODINGCHECK "Build encodingcheck" OFF)

add_subdirectory(src)

if(BUILD_ENCODINGCHECK)
    add_subdirectory(.github/workflows/encodingcheck)
endif()
