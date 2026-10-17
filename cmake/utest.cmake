function(utest_discover_tests target)
    set(script [==[
execute_process(COMMAND "$<TARGET_FILE:@target@>" --list-tests OUTPUT_VARIABLE names RESULT_VARIABLE result)
if(NOT result EQUAL 0)
    message(WARNING "@target@ cannot list its tests, build it first")
    return()
endif()
string(REPLACE "\n" ";" names "${names}")
foreach(name IN LISTS names)
    if(NOT name STREQUAL "")
        add_test("${name}" "$<TARGET_FILE:@target@>" "--filter=${name}")
        set_tests_properties("${name}" PROPERTIES SKIP_REGULAR_EXPRESSION "\\[  SKIPPED \\]")
    endif()
endforeach()
]==])
    string(CONFIGURE "${script}" script @ONLY)

    file(GENERATE OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/${target}_$<CONFIG>.cmake CONTENT "${script}")

    get_property(multi_config GLOBAL PROPERTY GENERATOR_IS_MULTI_CONFIG)

    if(multi_config)
        file(WRITE ${CMAKE_CURRENT_BINARY_DIR}/${target}_include.cmake "include(\"${CMAKE_CURRENT_BINARY_DIR}/${target}_\${CTEST_CONFIGURATION_TYPE}.cmake\" OPTIONAL)\n")
    else()
        file(WRITE ${CMAKE_CURRENT_BINARY_DIR}/${target}_include.cmake "include(\"${CMAKE_CURRENT_BINARY_DIR}/${target}_${CMAKE_BUILD_TYPE}.cmake\")\n")
    endif()

    set_property(DIRECTORY APPEND PROPERTY TEST_INCLUDE_FILES ${CMAKE_CURRENT_BINARY_DIR}/${target}_include.cmake)
endfunction()
