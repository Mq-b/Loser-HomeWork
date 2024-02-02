find_program(POWERSHELL_EXECUTABLE NAMES powershell)
mark_as_advanced(POWERSHELL_EXECUTABLE)

INCLUDE(FindPackageHandleStandardArgs)
find_package_handle_standard_args(PowerShell
        REQUIRED_VARS POWERSHELL_EXECUTABLE)