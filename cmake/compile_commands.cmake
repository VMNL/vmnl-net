cmake_path(GET PROJECT_BINARY_DIR PARENT_PATH build_dir)

if(NOT CMAKE_GENERATOR MATCHES "Ninja|Makefiles" OR NOT build_dir STREQUAL "${PROJECT_SOURCE_DIR}/build")
    return()
endif()

add_custom_target(compile_commands ALL
  COMMAND ${CMAKE_COMMAND} -E copy_if_different ${PROJECT_BINARY_DIR}/compile_commands.json ${build_dir}/compile_commands.json
  VERBATIM
)
