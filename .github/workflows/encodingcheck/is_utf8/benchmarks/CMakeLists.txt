
include(${PROJECT_SOURCE_DIR}/cmake/import.cmake)
set_off(SIMDUTF_TOOLS)
set_off(SIMDUTF_BENCHMARKS)

import_dependency(simdutf simdutf/simdutf v2.0.9)
set(BUILD_TESTING OFF)
add_dependency(simdutf)
unset(BUILD_TESTING)

add_executable(bench bench.cpp)
target_link_libraries(bench PRIVATE is_utf8)
target_link_libraries(bench PRIVATE simdutf)
