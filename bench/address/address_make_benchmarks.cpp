#include <vmnl/net/address.h>

#include <benchmark/benchmark.h>

static void address_make(benchmark::State &state, const char *ip_address)
{
    VmnlNetAddress address;

    for (auto _ : state) {
        benchmark::DoNotOptimize(vmnl_net_address_make(ip_address, 8080, &address, NULL));
    }
}

BENCHMARK_CAPTURE(address_make, ipv4, "192.0.2.1");
BENCHMARK_CAPTURE(address_make, ipv6, "2001:db8::1");
BENCHMARK_CAPTURE(address_make, longest_ipv6, "ffff:ffff:ffff:ffff:ffff:ffff:ffff:ffff");
BENCHMARK_CAPTURE(address_make, mapped_text, "::ffff:192.0.2.1");
BENCHMARK_CAPTURE(address_make, invalid, "localhost");
