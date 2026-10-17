#include <vmnl/net/buffer.h>

#include <benchmark/benchmark.h>

static void buffer_reserve_dynamic(benchmark::State &state)
{
    state.SkipWithMessage("not written");
}

BENCHMARK(buffer_reserve_dynamic);

static void buffer_reserve_ring(benchmark::State &state)
{
    state.SkipWithMessage("not written");
}

BENCHMARK(buffer_reserve_ring);

static void buffer_reserve_chunks(benchmark::State &state)
{
    state.SkipWithMessage("not written");
}

BENCHMARK(buffer_reserve_chunks);
