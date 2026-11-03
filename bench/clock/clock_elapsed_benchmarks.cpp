#include <vmnl/net/clock.h>

#include <benchmark/benchmark.h>

static void clock_elapsed(benchmark::State &state)
{
    const uint64_t start = vmnl_net_clock_now();

    for (auto _ : state) {
        benchmark::DoNotOptimize(vmnl_net_clock_elapsed(start));
    }
}

BENCHMARK(clock_elapsed);
