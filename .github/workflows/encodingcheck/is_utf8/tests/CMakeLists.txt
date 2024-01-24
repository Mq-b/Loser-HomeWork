if(MSVC)
  add_compile_options("/Zi" "/EHsc" "/GR")
endif()

include(${PROJECT_SOURCE_DIR}/cmake/add_cpp_test.cmake)
link_libraries(is_utf8)

add_cpp_test(unit)
