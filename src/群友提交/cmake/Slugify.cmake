cmake_minimum_required(VERSION 3.17)

find_package(Python COMPONENTS Interpreter REQUIRED)

function(slugify str _retname)
    string(MD5 strhash ${str})
    file(WRITE ${CMAKE_BINARY_DIR}/slugify-${strhash}.txt "${str}")
    execute_process(
        COMMAND ${PYTHON_EXECUTABLE} "-m" "slugify" "--stdin"
        TIMEOUT 10
        INPUT_FILE ${CMAKE_BINARY_DIR}/slugify-${strhash}.txt
        OUTPUT_VARIABLE sluggish
        COMMAND_ERROR_IS_FATAL ANY
    )
    set(${_retname} ${sluggish} PARENT_SCOPE)
endfunction()
