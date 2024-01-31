cmake_minimum_required(VERSION 3.17)
project(loser_homework)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH}
        "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

add_subdirectory(src)
add_subdirectory(.github/workflows/encodingcheck)