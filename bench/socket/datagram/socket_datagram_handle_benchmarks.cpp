#include "socket_datagram_benchmarks.hpp"

static void socket_datagram_handle(benchmark::State &state)
{
    Sockets sockets(state, udp_ipv4, 1);

    for (auto _ : state) {
        benchmark::DoNotOptimize(vmnl_net_socket_datagram_handle(sockets[0]));
    }
}

BENCHMARK(socket_datagram_handle);
