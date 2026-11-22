find_program(GCOVR gcovr)

if(NOT GCOVR)
    return()
endif()

if(CMAKE_C_COMPILER_ID STREQUAL "AppleClang")
    set(gcov_executable --gcov-executable "xcrun llvm-cov gcov")
elseif(CMAKE_C_COMPILER_ID STREQUAL "Clang")
    set(gcov_executable --gcov-executable "llvm-cov gcov")
endif()

add_custom_target(coverage
  COMMAND ${CMAKE_COMMAND} -E make_directory ${PROJECT_BINARY_DIR}/report/html
  COMMAND ${GCOVR} --root . --filter src/ ${gcov_executable}
    --html-details ${PROJECT_BINARY_DIR}/report/html/index.html
    --markdown ${PROJECT_BINARY_DIR}/report/summary.md
    --fail-under-line 100 --fail-under-branch 100
    ${PROJECT_BINARY_DIR}
  WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
  VERBATIM
)
