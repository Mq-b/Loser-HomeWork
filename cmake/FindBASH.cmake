find_program(BASH_EXECUTABLE NAMES bash)
mark_as_advanced(BASH_EXECUTABLE)

INCLUDE(FindPackageHandleStandardArgs)
find_package_handle_standard_args(BASH
        REQUIRED_VARS BASH_EXECUTABLE)