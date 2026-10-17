#include <vmnl/net/version.h>

#include <benchmark/benchmark.h>

static void version(benchmark::State &state)
{
    state.SkipWithMessage("not written");
}

BENCHMARK(version);
