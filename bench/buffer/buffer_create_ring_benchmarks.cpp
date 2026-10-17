#include <vmnl/net/buffer.h>

#include <benchmark/benchmark.h>

static void buffer_create_ring(benchmark::State &state)
{
    state.SkipWithMessage("not written");
}

BENCHMARK(buffer_create_ring);
