#include <vmnl/net/buffer.h>

#include <benchmark/benchmark.h>

static void buffer_is_empty(benchmark::State &state)
{
    state.SkipWithMessage("not written");
}

BENCHMARK(buffer_is_empty);
