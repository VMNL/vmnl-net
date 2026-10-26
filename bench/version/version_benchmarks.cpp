#include <vmnl/net/version.h>

#include <benchmark/benchmark.h>

static void version(benchmark::State &state)
{
    for (auto _ : state) {
        benchmark::DoNotOptimize(vmnl_net_version());
    }
}

BENCHMARK(version);
