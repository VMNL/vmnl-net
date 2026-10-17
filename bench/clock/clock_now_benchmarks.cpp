#include <vmnl/net/clock.h>

#include <benchmark/benchmark.h>

static void clock_now(benchmark::State &state)
{
    state.SkipWithMessage("not written");
}

BENCHMARK(clock_now);
