cmake_minimum_required(VERSION 3.17)

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
    if(NOT name MATCHES "^[a-zA-Z0-9_\\-\\+\\.]+$")
        set(ret false)
    endif ()
    set(${_ret} ${ret} PARENT_SCOPE)
endfunction()

function(get_homework_target_name question_index cpp_file_path _target_name)
    get_filename_component(file_name ${cpp_file_path} NAME_WLE)
    is_valid_target_name(${file_name} ret)
    if (NOT ret)
        message(NOTICE "not valid target name: ${file_name}")
        string(MD5 hash_file_name ${file_name})
        set(target_name "${question_index}_${hash_file_name}")
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

function(add_homework_target target question_index cpp_file_path)
    message(STATUS "add executable: ${target_name} ${cpp_file}")
    add_executable (${target} ${cpp_file})
endfunction()

function(handle_homework homework_dir _targets_list)
    set(targets_list "")
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
                endif ()
            endforeach ()
        endif ()
    endforeach()
    set(${_targets_list} ${targets_list} PARENT_SCOPE)
endfunction()

function(enable_msvc_build_stl_modules)
    if( MSVC_VERSION GREATER_EQUAL 1936 AND MSVC_IDE ) # 17.6+
        # When using /std:c++latest, "Build ISO C++23 Standard Library Modules" defaults to "Yes".
        # Default to "No" instead.
        #
        # As of CMake 3.26.4, there isn't a way to control this property
        # (https://gitlab.kitware.com/cmake/cmake/-/issues/24922),
        # We'll use the MSBuild project system instead
        # (https://learn.microsoft.com/en-us/cpp/build/reference/vcxproj-file-structure)
        file( CONFIGURE OUTPUT "${CMAKE_BINARY_DIR}/Directory.Build.props" CONTENT [==[
<Project>
  <ItemDefinitionGroup>
    <ClCompile>
      <BuildStlModules>true</BuildStlModules>
    </ClCompile>
  </ItemDefinitionGroup>
</Project>
]==] @ONLY )
    endif()
endfunction()