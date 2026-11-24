find_program(MULL_RUNNER mull-runner)

if(NOT MULL_RUNNER)
    return()
endif()

add_custom_target(mutation
  COMMAND sh -c "ulimit -v 2097152 2> /dev/null; exec \"$@\"" sh
    ${MULL_RUNNER} $<TARGET_FILE:vmnl_net_tests> --timeout 30000 --allow-surviving
    --reporters Elements --report-dir ${PROJECT_BINARY_DIR}/report/html --report-name mutation
  WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
  VERBATIM
)
