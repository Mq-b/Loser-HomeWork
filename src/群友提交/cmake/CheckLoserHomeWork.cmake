cmake_minimum_required(VERSION 3.17)

include(Slugify)

function(get_subdirs path _subdirs_list)
    file(GLOB all_childs
            CONFIGURE_DEPENDS "${path}/*")
    set(subdirs_list "")
    foreach(child ${all_childs})
        if(IS_DIRECTORY ${child})
            list(APPEND subdirs_list ${child})
        endif()
    endforeach()
    set(${_subdirs_list} ${subdirs_list} PARENT_SCOPE)
endfunction()

function(to_question_index dir_path _index)
    get_filename_component(dir_name ${dir_path} NAME)
    string(REGEX MATCH "[0-9]+" index ${dir_name})
    set(${_index} ${index} PARENT_SCOPE)
endfunction()

function(get_cpp_files path _file_list)
    file(GLOB file_list
            CONFIGURE_DEPENDS "${path}/*.cpp"
            "${path}/*.cxx" "${path}/*.cc")
    set(${_file_list} ${file_list} PARENT_SCOPE)
endfunction()

function(is_valid_target_name name _ret)
    set(ret true)
    if(NOT name MATCHES "^[a-zA-Z0-9_\+\.\-]+$")
        set(ret false)
    endif ()
    set(${_ret} ${ret} PARENT_SCOPE)
endfunction()

function(get_homework_target_name question_index cpp_file_path _target_name)
    get_filename_component(file_name ${cpp_file_path} NAME_WLE)
    is_valid_target_name("${file_name}" ret)
    if (NOT ret)
        slugify("${file_name}" slug)
        message(NOTICE "not valid target name: ${file_name}, use: ${slug}")
        set(target_name "${question_index}_${slug}")
    else ()
        set(target_name "${question_index}_${file_name}")
    endif ()
    set(${_target_name} ${target_name} PARENT_SCOPE)
endfunction()

function(is_target_compiler cpp_file _ret)
    file(STRINGS ${cpp_file} lines)
    list(POP_FRONT lines first_line)
    string(REGEX MATCHALL "(!gcc|!msvc|!clang|!all)" match_result ${first_line})
    list(FIND match_result "!all" index)
    if (NOT index EQUAL -1)
        set(${_ret} false PARENT_SCOPE)
        return()
    endif ()

    if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
        list(FIND match_result "!clang" index)
    elseif (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
        list(FIND match_result "!gcc" index)
    elseif (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
        list(FIND match_result "!msvc" index)
    endif()
    if (NOT index EQUAL -1)
        set(${_ret} false PARENT_SCOPE)
        return()
    endif ()

    set(${_ret} true PARENT_SCOPE)
endfunction()

function(is_use_modules cpp_file _ret)
    set(${_ret} false PARENT_SCOPE)
    file(STRINGS ${cpp_file} lines)
    if("${lines}" MATCHES "import")
        set(${_ret} true PARENT_SCOPE)
    endif ()
endfunction()

function(add_homework_target target question_index cpp_file)
    message(STATUS "add executable: ${target_name} ${cpp_file}")
    add_executable (${target} ${cpp_file})
    set_target_properties(${target}
            PROPERTIES cpp_file ${cpp_file})
endfunction()

function(is_check_run cpp_file _ret)
    file(STRINGS ${cpp_file} lines)
    list(POP_FRONT lines first_line)
    string(REGEX MATCHALL "(!check-run)" match_result ${first_line})
    list(FIND match_result "!check-run" index)
    if (NOT index EQUAL -1)
        set(${_ret} false PARENT_SCOPE)
        return()
    endif ()
    set(${_ret} true PARENT_SCOPE)
endfunction()

find_package(Powershell)
find_package(BASH)

function(add_run_homework_target target homework_target _ret)
    set(ret false)
    get_target_property(cpp_file ${homework_target} cpp_file)
    get_filename_component(file_dir ${cpp_file} DIRECTORY)
    set(input_file "${file_dir}/input.txt")
    set(output_file "${file_dir}/output.txt")
    if(EXISTS ${output_file})
        if (POWERSHELL_FOUND)
            add_custom_target(${target}
                    COMMAND powershell -File
                    ${CMAKE_CURRENT_SOURCE_DIR}/script/RunHomework.ps1
                    -Execution $<TARGET_FILE:${homework_target}>
                    -OutputName ${homework_target}
                    -CorrectExecOutput ${output_file}
                    -ExecInput ${input_file}
                    COMMENT "check ${homework_target} run"
                    DEPENDS ${target}
                    USES_TERMINAL
            )
            set(ret true)
        elseif (BASH_FOUND)
            add_custom_target(${target}
                    COMMAND bash
                    ${CMAKE_CURRENT_SOURCE_DIR}/script/run_homework.sh
                    -e $<TARGET_FILE:${homework_target}>
                    -o ${homework_target}
                    -c ${output_file}
                    -i ${input_file}
                    COMMENT "check ${homework_target} run"
                    DEPENDS ${target}
            )
            set(ret true)
        endif ()
    endif ()

    set(${_ret} ${ret} PARENT_SCOPE)
endfunction()

function(handle_homework homework_dir _targets_list _run_targets_list)
    set(targets_list "")
    set(run_targets_list "")
    get_subdirs(${homework_dir} subdirs_list)
    foreach(subdir ${subdirs_list})
        to_question_index(${subdir} index)
        if (NOT ${index} STREQUAL "")
            get_cpp_files(${subdir} cpp_files_list)
            foreach(cpp_file ${cpp_files_list})
                is_target_compiler(${cpp_file} ret)
                if (ret)
                    get_homework_target_name(${index} ${cpp_file} target)
                    add_homework_target(${target} ${index} ${cpp_file})
                    list(APPEND targets_list ${target})
                    is_use_modules(${cpp_file} use_modules)
                    set_target_properties(${target}
                        PROPERTIES use_modules ${use_modules})
                    if(use_modules)
                        message(NOTICE "${target}: use modules")
                    endif()
                    
                    is_check_run(${cpp_file} check_run)
                    if(check_run)
                        add_run_homework_target("run_${target}" ${target} add_run_ret)
                        if (add_run_ret)
                            list(APPEND run_targets_list "run_${target}")
                        endif ()
                    endif ()
                endif ()
            endforeach ()
        endif ()
    endforeach()
    set(${_targets_list} ${targets_list} PARENT_SCOPE)
    set(${_run_targets_list} ${run_targets_list} PARENT_SCOPE)
endfunction()