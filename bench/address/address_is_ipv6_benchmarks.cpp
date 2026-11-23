#include "address_benchmarks.hpp"

static void address_is_ipv6(benchmark::State &state)
{
    const VmnlNetAddress address = make(state, "2001:db8::1");

    for (auto _ : state) {
        benchmark::DoNotOptimize(vmnl_net_address_is_ipv6(&address));
    }
}

BENCHMARK(address_is_ipv6);
