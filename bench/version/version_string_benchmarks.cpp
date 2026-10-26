#include <vmnl/net/version.h>

#include <benchmark/benchmark.h>

static void version_string(benchmark::State &state)
{
    for (auto _ : state) {
        benchmark::DoNotOptimize(vmnl_net_version_string());
    }
}

BENCHMARK(version_string);
