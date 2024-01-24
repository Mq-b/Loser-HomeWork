cmake_minimum_required(VERSION 3.17)
project(xdgmime LANGUAGES C)

add_library(libxdgmime STATIC xdgmime.c xdgmimeglob.c xdgmimeint.c xdgmimemagic.c xdgmimealias.c xdgmimeparent.c xdgmimecache.c xdgmimeicon.c)

add_executable(test-mime test-mime.c)
target_link_libraries(test-mime PRIVATE libxdgmime)

add_executable(test-mime-data test-mime-data.c)
target_link_libraries(test-mime-data PRIVATE libxdgmime)

add_executable(print-mime-data print-mime-data.c)
target_link_libraries(print-mime-data PRIVATE libxdgmime)

target_include_directories(libxdgmime PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_compile_definitions(libxdgmime PRIVATE HAVE_MMAP)