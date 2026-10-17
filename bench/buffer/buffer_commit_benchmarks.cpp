#include <vmnl/net/buffer.h>

#include <benchmark/benchmark.h>

static void buffer_commit_flat(benchmark::State &state)
{
    state.SkipWithMessage("not written");
}

BENCHMARK(buffer_commit_flat);

static void buffer_commit_dynamic(benchmark::State &state)
{
    state.SkipWithMessage("not written");
}

BENCHMARK(buffer_commit_dynamic);

static void buffer_commit_ring(benchmark::State &state)
{
    state.SkipWithMessage("not written");
}

BENCHMARK(buffer_commit_ring);

static void buffer_commit_chunks(benchmark::State &state)
{
    state.SkipWithMessage("not written");
}

BENCHMARK(buffer_commit_chunks);
