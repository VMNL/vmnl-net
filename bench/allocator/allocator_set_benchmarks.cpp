#include <vmnl/net/allocator.h>

#include <benchmark/benchmark.h>

#include <cstdlib>

static void allocator_set(benchmark::State &state)
{
    for (auto _ : state) {
        vmnl_net_allocator_set(malloc, realloc, free);
    }
}

BENCHMARK(allocator_set);
