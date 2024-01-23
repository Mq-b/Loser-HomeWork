cmake_minimum_required(VERSION 3.17)
project(check_loser_homework)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH}
        ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
include(CheckLoserHomeWork)

set(CMAKE_CXX_STANDARD 26)

if (MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /permissive-")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /permissive-")
    add_compile_options("$<$<C_COMPILER_ID:MSVC>:/utf-8>")
    add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")
    enable_msvc_build_stl_modules()
endif ()

set(homework_dir ${CMAKE_CURRENT_SOURCE_DIR})
handle_homework(${homework_dir} targets_list run_targets_list)

if (MSVC)
    foreach (target ${targets_list})
        target_compile_options(${target} PRIVATE "/experimental:module")
    endforeach ()
endif ()

add_custom_target(build_loser_homework)
if(targets_list)
    add_dependencies(build_loser_homework ${targets_list})
endif ()

add_custom_target(run_loser_homework)
if(run_targets_list)
    add_dependencies(run_loser_homework ${run_targets_list})
endif ()