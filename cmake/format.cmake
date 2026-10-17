find_program(CLANG_FORMAT clang-format)

if(NOT CLANG_FORMAT)
    return()
endif()

file(GLOB_RECURSE format_files CONFIGURE_DEPENDS
    ${PROJECT_SOURCE_DIR}/include/*.h
    ${PROJECT_SOURCE_DIR}/src/*.c
    ${PROJECT_SOURCE_DIR}/src/*.h
    ${PROJECT_SOURCE_DIR}/tests/*.c
    ${PROJECT_SOURCE_DIR}/tests/*.cpp
    ${PROJECT_SOURCE_DIR}/examples/*.c
)

add_custom_target(format
  COMMAND ${CLANG_FORMAT} -i ${format_files}
  VERBATIM
)

add_custom_target(format-check
  COMMAND ${CLANG_FORMAT} --dry-run --Werror ${format_files}
  VERBATIM
)
