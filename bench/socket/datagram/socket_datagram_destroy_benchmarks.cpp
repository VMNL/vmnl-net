#include "socket_datagram_benchmarks.hpp"

static void socket_datagram_destroy(benchmark::State &state, const DatagramType &type)
{
    Sockets context(state, type, 0);
    std::vector<VmnlNetSocketDatagram *> sockets(DATAGRAM_BATCH);

    for (auto _ : state) {
        state.PauseTiming();
        for (size_t it = 0; it < sockets.size(); it++) {
            sockets[it] = vmnl_net_socket_datagram_create(type.type, type.version, NULL);
        }
        state.ResumeTiming();
        for (const auto &netsock : sockets) {
            vmnl_net_socket_datagram_destroy(netsock);
        }
    }
    state.counters["destroy"] = benchmark::Counter(DATAGRAM_BATCH, benchmark::Counter::kIsIterationInvariantRate | benchmark::Counter::kInvert);
}

BENCHMARK_CAPTURE(socket_datagram_destroy, udp_ipv4, udp_ipv4);
BENCHMARK_CAPTURE(socket_datagram_destroy, udp_ipv6, udp_ipv6);
