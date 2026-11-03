#include <vmnl/net/clock.h>

#include <benchmark/benchmark.h>

static void clock_now(benchmark::State &state)
{
    for (auto _ : state) {
        benchmark::DoNotOptimize(vmnl_net_clock_now());
    }
}

BENCHMARK(clock_now);
