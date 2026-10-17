#include <vmnl/net/buffer.h>

#include <benchmark/benchmark.h>

static void buffer_clear_flat(benchmark::State &state)
{
    state.SkipWithMessage("not written");
}

BENCHMARK(buffer_clear_flat);

static void buffer_clear_dynamic(benchmark::State &state)
{
    state.SkipWithMessage("not written");
}

BENCHMARK(buffer_clear_dynamic);

static void buffer_clear_ring(benchmark::State &state)
{
    state.SkipWithMessage("not written");
}

BENCHMARK(buffer_clear_ring);

static void buffer_clear_chunks(benchmark::State &state)
{
    state.SkipWithMessage("not written");
}

BENCHMARK(buffer_clear_chunks);
