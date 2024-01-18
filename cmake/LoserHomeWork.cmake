cmake_minimum_required(VERSION 3.17)

function(get_cpp_files path _file_list)
    file(GLOB file_list "${path}/*.cpp" "${path}/*.cxx" "${path}/*.cc")
    set(${_file_list} ${file_list} PARENT_SCOPE)
endfunction()

function(generate_cpp_files_targets_name cpp_file_list _target_list prefix)
    set(target_list "")
    foreach(cpp_file ${cpp_file_list})
        get_filename_component(file_name ${cpp_file} NAME_WLE ABSOLUTE)
        is_valid_target_name(${file_name} ret)
        if (NOT ret)
            message(NOTICE "not valid target name: ${file_name}")
            string(MD5 hash_file_name ${file_name})
            list(APPEND target_list "${prefix}_${hash_file_name}")
        else ()
            list(APPEND target_list "${prefix}_${file_name}")
        endif ()
    endforeach()
    set(${_target_list} ${target_list} PARENT_SCOPE)
endfunction()

function(is_valid_target_name name _ret)
    set(ret true)
    if(NOT name MATCHES "^[a-zA-Z0-9_\\-\\+\\.]+$")
        set(ret false)
    endif ()
    set(${_ret} ${ret} PARENT_SCOPE)
endfunction()

function(remove_not_valid_target_name target_list _result_list)
    foreach(target ${target_list})
        if(EXISTS ${target})
            get_filename_component(file_name ${target} NAME_WLE ABSOLUTE)
        else()
            set(file_name ${target})
        endif ()

        is_valid_target_name(${file_name} ret)
        if (NOT ret)
            message("not valid target name: ${target}")
            list(REMOVE_ITEM target_list ${target})
            continue()
        endif ()
    endforeach()
    set(${_result_list} ${target_list} PARENT_SCOPE)
endfunction()

function(remove_use_modules_cpp target_list _result_list)
    foreach(target ${target_list})
        file(READ ${target} file_content)
        string(FIND "${file_content}" "import" import_found)
        if(NOT import_found EQUAL -1)
            message(STATUS "use modules: ${target}")
            list(REMOVE_ITEM target_list ${target})
        endif()
    endforeach()
    set(${_result_list} ${target_list} PARENT_SCOPE)
endfunction()

function(add_cpp_executable_targets cpp_file_list target_list)
    list(LENGTH cpp_file_list cpp_list_len)
    list(LENGTH target_list target_list_len)
    if (NOT ${cpp_list_len} EQUAL ${target_list_len})
        message(FATAL_ERROR "target list length wrong")
    endif ()
    foreach(cpp_file target IN ZIP_LISTS cpp_file_list target_list)
        message(STATUS "add executable: ${target} ${cpp_file}")
        add_executable ("${target}" "${cpp_file}")
        if(MSVC)
            target_compile_options("${target}" PRIVATE "/experimental:module")
        endif()
    endforeach()
endfunction()

function(add_run_cpp_executable_targets cpp_file_list target_list _run_target_list)
    list(LENGTH cpp_file_list cpp_list_len)
    list(LENGTH target_list target_list_len)
    if (NOT ${cpp_list_len} EQUAL ${target_list_len})
        message(FATAL_ERROR "target list length wrong")
    endif ()
    set(run_target_list "")
    foreach(cpp_file target IN ZIP_LISTS cpp_file_list target_list)
        set(run_target "run_${target}")
        add_custom_target("${run_target}"
                COMMAND "${target}"
                COMMENT "run ${target}"
        )
        list(APPEND run_target_list "${run_target}")
    endforeach()
    set(${_run_target_list} ${run_target_list} PARENT_SCOPE)
endfunction()

function(add_run_all_targets all_targets_name target_list)
    add_custom_target(${all_targets_name})
    foreach(target ${target_list})
        add_dependencies(${all_targets_name}
                ${target}
        )
    endforeach()
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