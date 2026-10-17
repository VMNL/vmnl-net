#include <vmnl/net/context.h>

#include <benchmark/benchmark.h>

static void context_destroy(benchmark::State &state)
{
    state.SkipWithMessage("not written");
}

BENCHMARK(context_destroy);
