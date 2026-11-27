#include "socket_datagram_benchmarks.hpp"

static void socket_datagram_connect(benchmark::State &state, const DatagramType &type)
{
    Sockets sockets(state, type, 2);

    if (sockets.bind(state) == false) {
        return;
    }
    for (auto _ : state) {
        benchmark::DoNotOptimize(vmnl_net_socket_datagram_connect(sockets[0], &sockets.address(1), NULL));
    }
}

BENCHMARK_CAPTURE(socket_datagram_connect, udp_ipv4, udp_ipv4);
BENCHMARK_CAPTURE(socket_datagram_connect, udp_ipv6, udp_ipv6);
