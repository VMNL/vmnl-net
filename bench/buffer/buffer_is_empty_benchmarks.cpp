#include "buffer_benchmarks.hpp"

static void buffer_is_empty(benchmark::State &state)
{
    Buffers buffers(state, VMNL_NET_BUFFER_FLAT, BUFFER_CAPACITY, 1);

    for (auto _ : state) {
        benchmark::DoNotOptimize(vmnl_net_buffer_is_empty(buffers[0]));
    }
}

BENCHMARK(buffer_is_empty);
