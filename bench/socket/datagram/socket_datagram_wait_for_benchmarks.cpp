#include "socket_datagram_benchmarks.hpp"

static void socket_datagram_wait_for(benchmark::State &state, const DatagramType &type)
{
    Sockets sockets(state, type, 1);

    for (auto _ : state) {
        benchmark::DoNotOptimize(vmnl_net_socket_datagram_wait_for(sockets[0], VMNL_NET_SOCKET_WRITABLE, 0, NULL));
    }
}

BENCHMARK_CAPTURE(socket_datagram_wait_for, udp_ipv4, udp_ipv4);
BENCHMARK_CAPTURE(socket_datagram_wait_for, udp_ipv6, udp_ipv6);
