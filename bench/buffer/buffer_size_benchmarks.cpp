#include <vmnl/net/buffer.h>

#include <benchmark/benchmark.h>

static void buffer_size(benchmark::State &state)
{
    state.SkipWithMessage("not written");
}

BENCHMARK(buffer_size);
