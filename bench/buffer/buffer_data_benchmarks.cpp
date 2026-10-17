#include <vmnl/net/buffer.h>

#include <benchmark/benchmark.h>

static void buffer_data_flat(benchmark::State &state)
{
    state.SkipWithMessage("not written");
}

BENCHMARK(buffer_data_flat);

static void buffer_data_dynamic(benchmark::State &state)
{
    state.SkipWithMessage("not written");
}

BENCHMARK(buffer_data_dynamic);

static void buffer_data_ring(benchmark::State &state)
{
    state.SkipWithMessage("not written");
}

BENCHMARK(buffer_data_ring);

static void buffer_data_chunks(benchmark::State &state)
{
    state.SkipWithMessage("not written");
}

BENCHMARK(buffer_data_chunks);
