#include <vmnl/net/buffer.h>

#include <benchmark/benchmark.h>

static void buffer_destroy_flat(benchmark::State &state)
{
    state.SkipWithMessage("not written");
}

BENCHMARK(buffer_destroy_flat);

static void buffer_destroy_dynamic(benchmark::State &state)
{
    state.SkipWithMessage("not written");
}

BENCHMARK(buffer_destroy_dynamic);

static void buffer_destroy_ring(benchmark::State &state)
{
    state.SkipWithMessage("not written");
}

BENCHMARK(buffer_destroy_ring);

static void buffer_destroy_chunks(benchmark::State &state)
{
    state.SkipWithMessage("not written");
}

BENCHMARK(buffer_destroy_chunks);
