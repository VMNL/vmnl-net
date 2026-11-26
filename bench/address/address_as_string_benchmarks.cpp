#include "address_benchmarks.hpp"

static void address_as_string(benchmark::State &state, const char *ip_address)
{
    const VmnlNetAddress address = make(state, ip_address);
    char text[VMNL_NET_ADDRESS_AS_STRING_SIZE];

    for (auto _ : state) {
        benchmark::DoNotOptimize(vmnl_net_address_as_string(&address, text, sizeof text, NULL));
    }
}

BENCHMARK_CAPTURE(address_as_string, ipv4, "192.0.2.1");
BENCHMARK_CAPTURE(address_as_string, ipv6, "2001:db8::1");
BENCHMARK_CAPTURE(address_as_string, longest_ipv6, "ffff:ffff:ffff:ffff:ffff:ffff:ffff:ffff");
