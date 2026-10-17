#include <vmnl/net/buffer.h>

#include <benchmark/benchmark.h>

static void buffer_prepare_flat(benchmark::State &state)
{
    state.SkipWithMessage("not written");
}

BENCHMARK(buffer_prepare_flat);

static void buffer_prepare_dynamic(benchmark::State &state)
{
    state.SkipWithMessage("not written");
}

BENCHMARK(buffer_prepare_dynamic);

static void buffer_prepare_ring(benchmark::State &state)
{
    state.SkipWithMessage("not written");
}

BENCHMARK(buffer_prepare_ring);

static void buffer_prepare_chunks(benchmark::State &state)
{
    state.SkipWithMessage("not written");
}

BENCHMARK(buffer_prepare_chunks);
