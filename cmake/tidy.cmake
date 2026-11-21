find_program(CLANG_TIDY clang-tidy REQUIRED)

set(clang_tidy_command ${CLANG_TIDY} --experimental-custom-checks)

if(APPLE AND NOT CMAKE_OSX_SYSROOT)
    execute_process(
        COMMAND xcrun --show-sdk-path
        OUTPUT_VARIABLE sdk_path
        OUTPUT_STRIP_TRAILING_WHITESPACE
        COMMAND_ERROR_IS_FATAL ANY
    )
    list(APPEND clang_tidy_command --extra-arg=-isysroot${sdk_path})
endif()

if(VMNL_NET_WARNINGS_AS_ERRORS)
    list(APPEND clang_tidy_command --warnings-as-errors=*)
endif()

set_target_properties(vmnl_net PROPERTIES C_CLANG_TIDY "${clang_tidy_command}")
