cmake_minimum_required(VERSION 3.15)

project(is_utf8
  DESCRIPTION "Fast UTF-8 Validation"
  LANGUAGES CXX
  VERSION 1.3.1
)

include(GNUInstallDirs)
include(CTest)

option(IS_UTF8_SANITIZE "Sanitize addresses" OFF)

if (NOT CMAKE_BUILD_TYPE)
  message(STATUS "No build type selected, default to Release")
  if(IS_UTF8_SANITIZE)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Choose the type of build." FORCE)
  else()
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Choose the type of build." FORCE)
  endif()
endif()

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_MACOSX_RPATH OFF)

set(IS_UTF8_LIB_VERSION "1.3.1" CACHE STRING "is_utf8 library version")
set(IS_UTF8_LIB_SOVERSION "1" CACHE STRING "is_utf8 library soversion")

set(IS_UTF8_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
add_subdirectory(src)

if (BUILD_TESTING)
  message(STATUS "The tests are enabled.")
  add_subdirectory(tests)
else()
  message(STATUS "The tests are disabled.")
endif(BUILD_TESTING)


add_subdirectory(benchmarks)
# ---- Install rules ----
add_library(is_utf8::is_utf8 ALIAS is_utf8)

set_target_properties(
    is_utf8 PROPERTIES
    VERSION "${IS_UTF8_LIB_VERSION}"
    SOVERSION "${IS_UTF8_LIB_SOVERSION}"
    WINDOWS_EXPORT_ALL_SYMBOLS YES
)

include(CMakePackageConfigHelpers)
include(GNUInstallDirs)

install(
    FILES include/is_utf8.h
    DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}"
    COMPONENT is_utf8_Development
)

install(
    TARGETS is_utf8
    EXPORT is_utf8Targets
    RUNTIME COMPONENT is_utf8_Runtime
    LIBRARY COMPONENT is_utf8_Runtime
    NAMELINK_COMPONENT is_utf8_Development
    ARCHIVE COMPONENT is_utf8_Development
    INCLUDES DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}"
)

configure_file(cmake/is_utf8-config.cmake.in is_utf8-config.cmake @ONLY)

write_basic_package_version_file(
    is_utf8-config-version.cmake
    COMPATIBILITY SameMinorVersion
)

set(
    IS_UTF8_INSTALL_CMAKEDIR "${CMAKE_INSTALL_LIBDIR}/cmake/is_utf8"
    CACHE STRING "CMake package config location relative to the install prefix"
)
mark_as_advanced(IS_UTF8_INSTALL_CMAKEDIR)

install(
    FILES
    "${PROJECT_BINARY_DIR}/is_utf8-config.cmake"
    "${PROJECT_BINARY_DIR}/is_utf8-config-version.cmake"
    DESTINATION "${IS_UTF8_INSTALL_CMAKEDIR}"
    COMPONENT is_utf8_Development
)

#
# CPack
#
if(is_top_project)
  set(CPACK_PACKAGE_VENDOR "Daniel Lemire")
  set(CPACK_PACKAGE_CONTACT "lemire@gmail.com")
  set(CPACK_RESOURCE_FILE_LICENSE "${PROJECT_SOURCE_DIR}/LICENSE-MIT")
  set(CPACK_RPM_PACKAGE_LICENSE "${PROJECT_SOURCE_DIR}/LICENSE-MIT")
  set(CPACK_RESOURCE_FILE_README "${PROJECT_SOURCE_DIR}/README.md")
  set(CPACK_SOURCE_GENERATOR "TGZ;ZIP")
  include(CPack)
endif()

# ----
