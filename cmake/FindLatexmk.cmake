# Copyright 2020 Keith F Prussing
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are
# met:
#
# 1.  Redistributions of source code must retain the above copyright
#     notice, this list of conditions and the following disclaimer.
#
# 2.  Redistributions in binary form must reproduce the above copyright
#     notice, this list of conditions and the following disclaimer in
#     the documentation and/or other materials provided with the
#     distribution.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
# "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
# LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
# A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT
# HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
# SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
# LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
# DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
# THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
# (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
# OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

#[================================================================[.md:

<https://github.com/kprussing/FindLatexmk/blob/trunk/usage.rst>

#]================================================================]

cmake_minimum_required(VERSION 3.17 FATAL_ERROR)

# Don't do duplicate work if necessary
if (TARGET Latexmk::Latexmk)
    return()
endif()

include(Slugify)

# Define the function to locate latexmk
function(_latexmk__find_latexmk)
    # First, check if the environment specifies a valid latexmk
    if (DEFINED ENV{LATEXMK})
        execute_process(COMMAND $ENV{LATEXMK} -h
                        RESULT_VARIABLE res
                        OUTPUT_QUIET ERROR_QUIET)
        if (NOT res EQUAL "0")
            message(FATAL_ERROR "LATEXMK=$ENV{LATEXMK} is not valid")
        endif()
        set(Latexmk_Executable $ENV{LATEXMK} CACHE PATH
            "Latexmk executable path" FORCE)
        return()
    endif()

    # Check if latexmk has been loaded.
    if (NOT DEFINED LATEX_FOUND)
        find_package(LATEX REQUIRED)
    endif()
    if (NOT LATEX_FOUND)
        message(FATAL_ERROR "No underlying LaTeX found")
    endif()

    # Otherwise, scan known directories.  We do this by inspecting the
    # results of :module:`FindLATEX`.  Note, we only really care about
    # the main compilers.
    if (DEFINED Latexmk_DIR AND Latexmk_DIR)
        find_program(Latexmk_Executable latexmk
                     PATHS ${Latexmk_DIR}
                     NO_DEFAULT_PATH)
        return()
    else()
        set(dirs)
        foreach(compiler LATEX LUALATEX PDFLATEX XELATEX)
            if (LATEX_${compiler}_FOUND)
                get_filename_component(_dir ${${compiler}_COMPILER} DIRECTORY)
                list(APPEND dirs ${_dir})
            endif()
        endforeach()
    endif()
    list(REMOVE_DUPLICATES dirs)
    if (NOT dirs)
        message(FATAL_ERROR "No directories to search for latexmk")
    endif()

    # Now actually do the search
    find_program(Latexmk_Executable latexmk PATHS ${dirs})
endfunction()

# Locate the latexmk if we haven't already.  Note this does require the
# user to reset :variable:`Latexmk_Executable`.
if (NOT DEFINED Latexmk_Executable)
    _latexmk__find_latexmk()
endif()

# Make sure we found the executable
include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(Latexmk DEFAULT_MSG Latexmk_Executable)

# Create the official target.
add_executable(Latexmk::Latexmk IMPORTED)
set_property(TARGET Latexmk::Latexmk
             PROPERTY IMPORTED_LOCATION "${Latexmk_Executable}")

function(add_latex_document source)
    cmake_parse_arguments(PARSE_ARGV 1 latexmk "ALL" "ENGINE" "")
    # Split the root and extension from the target
    get_filename_component(root ${source} NAME_WE)

    # Determine the output flags and extension based on the given
    # ENGINE.
    if (latexmk_ENGINE MATCHES "PDFLATEX")
        set(ext "pdf")
        set(flags "-dvi-;-ps-;-pdf")
    elseif (latexmk_ENGINE MATCHES "LUALATEX")
        set(ext "pdf")
        set(flags "-dvi-;-ps-;-lualatex")
    elseif (latexmk_ENGINE MATCHES "XELATEX")
        set(ext "pdf")
        set(flags "-dvi-;-ps-;-xelatex")
    elseif (latexmk_ENGINE MATCHES "(LATEX|DVI)")
        set(ext "dvi")
        set(flags "-dvi;-ps-;-pdf-")
    elseif (latexmk_ENGINE MATCHES "PS")
        set(ext "ps")
        set(flags "-dvi-;-ps;-pdf-")
    elseif (latexmk_ENGINE MATCHES "")
        set(ext "pdf")
        set(flags "-dvi-;-ps-;-pdf")
    else()
        message(FATAL_ERROR "Unknown engine '${latexmk_ENGINE}'")
    endif()

    set(target ${root}.${ext})
    set(byproducts "${target};${root}.log;${root}.fdb_latexmk")

    # Extra outputs not reported by latexmk.
    if (latexmk_ENGINE MATCHES "XELATEX")
        list(APPEND byproducts ${root}.xdv)
    elseif (latexmk_ENGINE MATCHES "PS")
        list(APPEND byproducts ${root}.dvi)
    endif()

    # Run Latexmk to determine what it thinks the generated files will
    # be so we can add these as byproducts.
    execute_process(
        COMMAND ${Latexmk_Executable}
            ${Latexmk_FLAGS}
            -e "print join(';', @generated_exts)"
            -e "exit(0)"
            ${source}
        OUTPUT_VARIABLE exts
        ERROR_VARIABLE errors
        RESULT_VARIABLE result
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    )
    if (NOT result EQUAL "0")
        message("Error getting generated extensions: ${exts}")
        message("Error getting generated extensions: ${result}")
        message(FATAL_ERROR "Error getting generated extensions: ${errors}")
    endif()
    foreach(ext ${exts})
        list(APPEND byproducts ${root}.${ext})
    endforeach()

    # Finally, create the target.  Using :command`add_custom_target` is
    # always considered out of date so the dependency resolution is
    # pushed to latexmk.
    if (latexmk_ALL)
        set(_all ALL)
    else()
        set(_all)
    endif()

    slugify(${target} target_slug)
    add_custom_target(${target_slug} ${_all}
                      COMMAND Latexmk::Latexmk ${flags}
                              -output-directory=${CMAKE_CURRENT_BINARY_DIR}
                              ${Latexmk_FLAGS}
                              ${source}
                      DEPENDS ${source} ${latexmk_UNPARSED_ARGUMENTS}
                      WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
                      BYPRODUCTS ${byproducts}
                     )
endfunction()