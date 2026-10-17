#include <vmnl/net/allocator.h>

#include <benchmark/benchmark.h>

static void allocator_set(benchmark::State &state)
{
    state.SkipWithMessage("not written");
}

BENCHMARK(allocator_set);
