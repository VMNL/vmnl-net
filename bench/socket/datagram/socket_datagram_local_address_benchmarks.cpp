#include "socket_datagram_benchmarks.hpp"

static void socket_datagram_local_address(benchmark::State &state)
{
    Sockets sockets(state, udp_ipv4, 1);
    VmnlNetAddress address;

    if (sockets.bind(state) == false) {
        return;
    }
    for (auto _ : state) {
        benchmark::DoNotOptimize(vmnl_net_socket_datagram_local_address(sockets[0], &address, NULL));
    }
}

BENCHMARK(socket_datagram_local_address);
