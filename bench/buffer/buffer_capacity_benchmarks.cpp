#include <vmnl/net/buffer.h>

#include <benchmark/benchmark.h>

static void buffer_capacity(benchmark::State &state)
{
    state.SkipWithMessage("not written");
}

BENCHMARK(buffer_capacity);
