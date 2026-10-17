#include <vmnl/net/buffer.h>

#include <benchmark/benchmark.h>

static void buffer_consume_flat(benchmark::State &state)
{
    state.SkipWithMessage("not written");
}

BENCHMARK(buffer_consume_flat);

static void buffer_consume_dynamic(benchmark::State &state)
{
    state.SkipWithMessage("not written");
}

BENCHMARK(buffer_consume_dynamic);

static void buffer_consume_ring(benchmark::State &state)
{
    state.SkipWithMessage("not written");
}

BENCHMARK(buffer_consume_ring);

static void buffer_consume_chunks(benchmark::State &state)
{
    state.SkipWithMessage("not written");
}

BENCHMARK(buffer_consume_chunks);
