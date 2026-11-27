#include "socket_datagram_benchmarks.hpp"

static void socket_datagram_set_broadcast(benchmark::State &state)
{
    Sockets sockets(state, udp_ipv4, 1);

    for (auto _ : state) {
        benchmark::DoNotOptimize(vmnl_net_socket_datagram_set_broadcast(sockets[0], true, NULL));
    }
}

BENCHMARK(socket_datagram_set_broadcast);
