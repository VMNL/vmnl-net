#include "address_benchmarks.hpp"

static void address_is_equal(benchmark::State &state, const char *other_ip_address)
{
    const VmnlNetAddress address = make(state, "2001:db8::1");
    const VmnlNetAddress other = make(state, other_ip_address);

    for (auto _ : state) {
        benchmark::DoNotOptimize(vmnl_net_address_is_equal(&address, &other));
    }
}

BENCHMARK_CAPTURE(address_is_equal, same, "2001:db8::1");
BENCHMARK_CAPTURE(address_is_equal, other_ip, "2001:db8::2");
