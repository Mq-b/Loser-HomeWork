cmake_minimum_required(VERSION 3.17)

option(BUILD_DOCS "Build TeX docs" OFF)
option(BUILD_PROG "Build programs" ON)

if(BUILD_PROG)
    add_subdirectory(群友提交)
endif()

if(BUILD_DOCS)
    add_subdirectory(PDF版题目与答案)
endif()