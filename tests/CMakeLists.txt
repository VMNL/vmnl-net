enable_language(CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(FetchContent)
include(GoogleTest)

set(INSTALL_GTEST OFF)
set(gtest_force_shared_crt ON)

FetchContent_Declare(googletest
  URL https://github.com/google/googletest/archive/refs/tags/v1.18.0.tar.gz
  URL_HASH SHA256=6e3191c1455468b3fc35a417fb565c1c5071aee1b7e7f85e30cf48a98d37d8b5
  SYSTEM
  FIND_PACKAGE_ARGS NAMES GTest
)

block()
    set(CMAKE_EXPORT_COMPILE_COMMANDS OFF)
    FetchContent_MakeAvailable(googletest)
endblock()

add_executable(vmnl_net_tests)

target_sources(vmnl_net_tests
  PRIVATE
    core/vmnl_net_version_string_tests.cpp
    core/vmnl_net_version_tests.cpp
    error/vmnl_net_error_string_tests.cpp
    context/vmnl_net_context_create_tests.cpp
    context/vmnl_net_context_create_with_allocator_tests.cpp
    context/vmnl_net_context_destroy_tests.cpp
    buffer/vmnl_net_buffer_capacity_tests.cpp
    buffer/vmnl_net_buffer_claim_tests.cpp
    buffer/vmnl_net_buffer_clear_tests.cpp
    buffer/vmnl_net_buffer_commit_tests.cpp
    buffer/vmnl_net_buffer_consume_tests.cpp
    buffer/vmnl_net_buffer_create_dynamic_tests.cpp
    buffer/vmnl_net_buffer_create_ring_tests.cpp
    buffer/vmnl_net_buffer_destroy_tests.cpp
    buffer/vmnl_net_buffer_peek_tests.cpp
    buffer/vmnl_net_buffer_read_tests.cpp
    buffer/vmnl_net_buffer_reserve_tests.cpp
    buffer/vmnl_net_buffer_size_tests.cpp
    buffer/vmnl_net_buffer_view_tests.cpp
    buffer/vmnl_net_buffer_write_tests.cpp
    clock/vmnl_net_clock_now_tests.cpp
    timer/vmnl_net_timer_create_tests.cpp
    timer/vmnl_net_timer_destroy_tests.cpp
    timer/vmnl_net_timer_remaining_tests.cpp
    timer/vmnl_net_timer_reset_tests.cpp
    timer/vmnl_net_timer_start_tests.cpp
    timer/vmnl_net_timer_stop_tests.cpp
    timer/vmnl_net_timer_update_tests.cpp
)

target_link_libraries(vmnl_net_tests
  PRIVATE
    vmnl::net
    vmnl_net_warnings
    GTest::gtest_main
)

gtest_discover_tests(vmnl_net_tests DISCOVERY_MODE PRE_TEST)
