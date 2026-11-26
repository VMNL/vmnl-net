#include "address_benchmarks.hpp"

static void address_version(benchmark::State &state, const char *ip_address)
{
    const VmnlNetAddress address = make(state, ip_address);

    for (auto _ : state) {
        benchmark::DoNotOptimize(vmnl_net_address_version(&address));
    }
}

BENCHMARK_CAPTURE(address_version, ipv4, "192.0.2.1");
BENCHMARK_CAPTURE(address_version, ipv6, "2001:db8::1");
