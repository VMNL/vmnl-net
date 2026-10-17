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
    context/context_destroy_benchmarks.cpp
    clock/clock_elapsed_benchmarks.cpp
    clock/clock_now_benchmarks.cpp
    buffer/buffer_capacity_benchmarks.cpp
    buffer/buffer_clear_benchmarks.cpp
    buffer/buffer_commit_benchmarks.cpp
    buffer/buffer_consume_benchmarks.cpp
    buffer/buffer_create_chunks_benchmarks.cpp
    buffer/buffer_create_dynamic_benchmarks.cpp
    buffer/buffer_create_flat_benchmarks.cpp
    buffer/buffer_create_ring_benchmarks.cpp
    buffer/buffer_data_benchmarks.cpp
    buffer/buffer_destroy_benchmarks.cpp
    buffer/buffer_is_empty_benchmarks.cpp
    buffer/buffer_prepare_benchmarks.cpp
    buffer/buffer_reserve_benchmarks.cpp
    buffer/buffer_shrink_benchmarks.cpp
    buffer/buffer_size_benchmarks.cpp
)

target_link_libraries(vmnl_net_benchmarks
  PRIVATE
    vmnl::net
    vmnl_net_warnings
    benchmark::benchmark
)
