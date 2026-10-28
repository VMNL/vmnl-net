#include <vmnl/net/error.h>

#include <benchmark/benchmark.h>

static void error_as_string(benchmark::State &state)
{
    const auto error = static_cast<VmnlNetError>(state.range(0));

    for (auto _ : state) {
        benchmark::DoNotOptimize(vmnl_net_error_as_string(error));
    }
}

BENCHMARK(error_as_string)
    ->Args({VMNL_NET_EINVAL})
    ->Args({UINT32_MAX});
