cmake_minimum_required(VERSION 3.17)
project(encodingcheck LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 20)

include(FetchContent)

FetchContent_Declare(
    xdgmime
    GIT_REPOSITORY https://gitlab.freedesktop.org/xdg/xdgmime.git
    GIT_TAG master
)

FetchContent_Declare(
    is_utf8
    GIT_REPOSITORY https://github.com/simdutf/is_utf8.git
    GIT_TAG v1.3.1
)

FetchContent_MakeAvailable(xdgmime is_utf8)

message(xdgmime_SOURCE_DIR=${xdgmime_SOURCE_DIR})

set(xdgmime_SOURCE_DIR ${xdgmime_SOURCE_DIR})

include(cmake/xdgmime.cmake)

add_executable(encodingcheck encodingcheck.cpp)
target_link_libraries(encodingcheck PRIVATE libxdgmime is_utf8::is_utf8)
