#include "socket_datagram_benchmarks.hpp"

static void socket_datagram_set_receive_buffer_size(benchmark::State &state)
{
    Sockets sockets(state, udp_ipv4, 1);

    for (auto _ : state) {
        benchmark::DoNotOptimize(vmnl_net_socket_datagram_set_receive_buffer_size(sockets[0], DATAGRAM_BUFFER_SIZE, NULL));
    }
}

BENCHMARK(socket_datagram_set_receive_buffer_size);
