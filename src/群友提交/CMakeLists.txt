cmake_minimum_required(VERSION 3.17)
project(check_loser_homework)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH}
        ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
include(CheckLoserHomeWork)

set(CMAKE_CXX_STANDARD 26)

if (MSVC)
    set(CMAKE_CXX_SCAN_FOR_MODULES 1)
    set(CMAKE_EXPERIMENTAL_CXX_MODULE_DYNDEP 1)
    set(CMAKE_EXPERIMENTAL_CXX_MODULE_CMAKE_API "aa1f7df0-828a-4fcd-9afc-2dc80491aca7") #for cmake 3.27
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /permissive-")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /permissive-")
    add_compile_options("$<$<C_COMPILER_ID:MSVC>:/utf-8>")
    add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")
    add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/EHsc>")

    string(REGEX REPLACE "\/bin\/Host(x|X)(64|86)\/x(64|86)\/cl\.exe" "" MSVC_ROOT "${CMAKE_CXX_COMPILER}")
    # std.ixx
    configure_file("${MSVC_ROOT}/modules/std.ixx" "${PROJECT_BINARY_DIR}/std.ixx" COPYONLY)
    # std.compat.ixx
    configure_file("${MSVC_ROOT}/modules/std.compat.ixx" "${PROJECT_BINARY_DIR}/std.compat.ixx" COPYONLY)

    add_library(std)
    target_sources(std
            PUBLIC
            FILE_SET cxx_modules TYPE CXX_MODULES BASE_DIRS ${PROJECT_BINARY_DIR}
            FILES ${PROJECT_BINARY_DIR}/std.ixx ${PROJECT_BINARY_DIR}/std.compat.ixx
    )
endif ()

set(homework_dir ${CMAKE_CURRENT_SOURCE_DIR})
handle_homework(${homework_dir} targets_list run_targets_list)

if (MSVC)
    foreach (target ${targets_list})
        get_target_property(use_modules ${target} use_modules)
        if(use_modules)
            target_compile_options(${target} PRIVATE "/experimental:module")
            target_link_libraries(${target}
                PRIVATE
                std
            )
        endif(use_modules)
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