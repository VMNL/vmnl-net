#include <vmnl/net/buffer.h>

#include <benchmark/benchmark.h>

static void buffer_shrink_dynamic(benchmark::State &state)
{
    state.SkipWithMessage("not written");
}

BENCHMARK(buffer_shrink_dynamic);

static void buffer_shrink_ring(benchmark::State &state)
{
    state.SkipWithMessage("not written");
}

BENCHMARK(buffer_shrink_ring);

static void buffer_shrink_chunks(benchmark::State &state)
{
    state.SkipWithMessage("not written");
}

BENCHMARK(buffer_shrink_chunks);
