cmake_minimum_required(VERSION 3.17)
project(extxdgmime LANGUAGES C)

add_library(libxdgmime STATIC
    ${xdgmime_SOURCE_DIR}/src/xdgmime.c
    ${xdgmime_SOURCE_DIR}/src/xdgmimeglob.c
    ${xdgmime_SOURCE_DIR}/src/xdgmimeint.c
    ${xdgmime_SOURCE_DIR}/src/xdgmimemagic.c
    ${xdgmime_SOURCE_DIR}/src/xdgmimealias.c
    ${xdgmime_SOURCE_DIR}/src/xdgmimeparent.c
    ${xdgmime_SOURCE_DIR}/src/xdgmimecache.c
    ${xdgmime_SOURCE_DIR}/src/xdgmimeicon.c
)

add_executable(test-mime ${xdgmime_SOURCE_DIR}/src/test-mime.c)
target_link_libraries(test-mime PRIVATE libxdgmime)

add_executable(test-mime-data ${xdgmime_SOURCE_DIR}/src/test-mime-data.c)
target_link_libraries(test-mime-data PRIVATE libxdgmime)

add_executable(print-mime-data ${xdgmime_SOURCE_DIR}/src/print-mime-data.c)
target_link_libraries(print-mime-data PRIVATE libxdgmime)

target_include_directories(libxdgmime PUBLIC ${xdgmime_SOURCE_DIR}/src/)
target_compile_definitions(libxdgmime PRIVATE HAVE_MMAP)