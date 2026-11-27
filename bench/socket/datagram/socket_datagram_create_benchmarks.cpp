#include "socket_datagram_benchmarks.hpp"

static void socket_datagram_create(benchmark::State &state, const DatagramType &type)
{
    Sockets context(state, type, 0);
    std::vector<VmnlNetSocketDatagram *> sockets(DATAGRAM_BATCH);

    for (auto _ : state) {
        for (size_t it = 0; it < sockets.size(); it++) {
            sockets[it] = vmnl_net_socket_datagram_create(type.type, type.version, NULL);
        }
        state.PauseTiming();
        for (const auto &netsock : sockets) {
            vmnl_net_socket_datagram_destroy(netsock);
        }
        state.ResumeTiming();
    }
    state.counters["create"] = benchmark::Counter(DATAGRAM_BATCH, benchmark::Counter::kIsIterationInvariantRate | benchmark::Counter::kInvert);
}

BENCHMARK_CAPTURE(socket_datagram_create, udp_ipv4, udp_ipv4);
BENCHMARK_CAPTURE(socket_datagram_create, udp_ipv6, udp_ipv6);
