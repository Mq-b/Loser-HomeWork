cmake_minimum_required(VERSION 3.17)
cmake_policy(SET CMP0144 NEW)

find_package(Python COMPONENTS Interpreter REQUIRED)

function(slugify str _retname)
    string(MD5 strhash ${str})
    file(WRITE ${CMAKE_BINARY_DIR}/slugify-${strhash}.txt "${str}")
    execute_process(
        COMMAND ${Python_EXECUTABLE} "-m" "slugify" "--stdin"
        TIMEOUT 10
        INPUT_FILE ${CMAKE_BINARY_DIR}/slugify-${strhash}.txt
        OUTPUT_VARIABLE sluggish
        COMMAND_ERROR_IS_FATAL ANY
    )
    string(STRIP ${sluggish} sluggish)
    set(${_retname} ${sluggish} PARENT_SCOPE)
endfunction()
