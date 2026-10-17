#include <vmnl/net/error.h>

#include <benchmark/benchmark.h>

static void error_as_string(benchmark::State &state)
{
    state.SkipWithMessage("not written");
}

BENCHMARK(error_as_string);
