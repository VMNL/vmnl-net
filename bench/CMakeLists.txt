enable_language(CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(FetchContent)

set(BENCHMARK_ENABLE_TESTING OFF)
set(BENCHMARK_ENABLE_GTEST_TESTS OFF)
set(BENCHMARK_ENABLE_INSTALL OFF)
set(BENCHMARK_INSTALL_DOCS OFF)
set(BENCHMARK_ENABLE_WERROR OFF)

FetchContent_Declare(benchmark
  URL https://github.com/google/benchmark/archive/refs/tags/v1.9.5.tar.gz
  URL_HASH SHA256=9631341c82bac4a288bef951f8b26b41f69021794184ece969f8473977eaa340
  SYSTEM
  FIND_PACKAGE_ARGS NAMES benchmark
)

block()
    set(CMAKE_EXPORT_COMPILE_COMMANDS OFF)
    FetchContent_MakeAvailable(benchmark)
endblock()

add_executable(vmnl_net_benchmarks)

target_sources(vmnl_net_benchmarks
  PRIVATE
    main.cpp
    version/version_benchmarks.cpp
    version/version_string_benchmarks.cpp
    error/error_as_string_benchmarks.cpp
    allocator/allocator_set_benchmarks.cpp
    context/context_create_benchmarks.cpp
    context/context_remaining_benchmarks.cpp
    context/context_advance_benchmarks.cpp
    context/context_destroy_benchmarks.cpp
    clock/clock_now_benchmarks.cpp
    clock/clock_elapsed_benchmarks.cpp
    timer/timer_create_benchmarks.cpp
    timer/timer_is_active_benchmarks.cpp
    timer/timer_interval_benchmarks.cpp
    timer/timer_remaining_benchmarks.cpp
    timer/timer_set_callback_benchmarks.cpp
    timer/timer_set_interval_benchmarks.cpp
    timer/timer_start_benchmarks.cpp
    timer/timer_stop_benchmarks.cpp
    timer/timer_destroy_benchmarks.cpp
    buffer/buffer_create_benchmarks.cpp
    buffer/buffer_size_benchmarks.cpp
    buffer/buffer_capacity_benchmarks.cpp
    buffer/buffer_is_empty_benchmarks.cpp
    buffer/buffer_clear_benchmarks.cpp
    buffer/buffer_resize_benchmarks.cpp
    buffer/buffer_shrink_benchmarks.cpp
    buffer/buffer_prepare_benchmarks.cpp
    buffer/buffer_commit_benchmarks.cpp
    buffer/buffer_data_benchmarks.cpp
    buffer/buffer_consume_benchmarks.cpp
    buffer/buffer_destroy_benchmarks.cpp
    buffer/buffer_cycle_benchmarks.cpp
    address/address_make_benchmarks.cpp
    address/address_version_benchmarks.cpp
    address/address_is_equal_benchmarks.cpp
    address/address_as_string_benchmarks.cpp
    socket/datagram/socket_datagram_create_benchmarks.cpp
    socket/datagram/socket_datagram_bind_benchmarks.cpp
    socket/datagram/socket_datagram_connect_benchmarks.cpp
    socket/datagram/socket_datagram_set_broadcast_benchmarks.cpp
    socket/datagram/socket_datagram_set_receive_buffer_size_benchmarks.cpp
    socket/datagram/socket_datagram_set_send_buffer_size_benchmarks.cpp
    socket/datagram/socket_datagram_local_address_benchmarks.cpp
    socket/datagram/socket_datagram_handle_benchmarks.cpp
    socket/datagram/socket_datagram_wait_for_benchmarks.cpp
    socket/datagram/socket_datagram_send_benchmarks.cpp
    socket/datagram/socket_datagram_send_to_benchmarks.cpp
    socket/datagram/socket_datagram_receive_benchmarks.cpp
    socket/datagram/socket_datagram_receive_from_benchmarks.cpp
    socket/datagram/socket_datagram_destroy_benchmarks.cpp
)

target_link_libraries(vmnl_net_benchmarks
  PRIVATE
    vmnl::net
    vmnl_net_warnings
    benchmark::benchmark
)
