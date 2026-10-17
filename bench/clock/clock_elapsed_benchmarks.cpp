#include <vmnl/net/clock.h>

#include <benchmark/benchmark.h>

static void clock_elapsed(benchmark::State &state)
{
    state.SkipWithMessage("not written");
}

BENCHMARK(clock_elapsed);
