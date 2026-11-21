#include "buffer_benchmarks.hpp"

static void buffer_size(benchmark::State &state)
{
    Buffers buffers(state, VMNL_NET_BUFFER_FLAT, BUFFER_CAPACITY, 1);

    for (auto _ : state) {
        benchmark::DoNotOptimize(vmnl_net_buffer_size(buffers[0]));
    }
}

BENCHMARK(buffer_size);
