#include <vmnl/net/version.h>

#include <benchmark/benchmark.h>

static void version_string(benchmark::State &state)
{
    state.SkipWithMessage("not written");
}

BENCHMARK(version_string);
