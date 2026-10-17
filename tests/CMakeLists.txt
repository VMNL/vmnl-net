include(FetchContent)
include(${PROJECT_SOURCE_DIR}/cmake/utest.cmake)

FetchContent_Declare(utest
  URL https://github.com/sheredom/utest.h/archive/df92c7589f7f1d1bd22489cc029e8a61ef538588.tar.gz
  URL_HASH SHA256=00e8918b42a8e10efaeb38ca7d6a9c32cea14b956e8d511a8379cd26288718b5
)

FetchContent_MakeAvailable(utest)

add_executable(vmnl_net_tests)

target_sources(vmnl_net_tests
  PRIVATE
    main.c
    version/version_tests.c
    version/version_string_tests.c
    error/error_as_string_tests.c
    allocator/allocator_set_tests.c
    context/context_create_tests.c
    context/context_destroy_tests.c
    clock/clock_elapsed_tests.c
    clock/clock_now_tests.c
)

target_include_directories(vmnl_net_tests SYSTEM PRIVATE ${utest_SOURCE_DIR})

target_link_libraries(vmnl_net_tests
  PRIVATE
    vmnl::net
    vmnl_net_warnings
)

utest_discover_tests(vmnl_net_tests)
