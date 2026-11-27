#include "socket_datagram_benchmarks.hpp"

static void socket_datagram_send(benchmark::State &state, const DatagramType &type)
{
    Sockets sockets(state, type, 2);
    std::vector<uint8_t> bytes(DATAGRAM_SIZE);
    VmnlNetBufferView view = {};

    view.data = bytes.data();
    view.size = (VmnlNetBufferViewSize) state.range(0);
    if (sockets.bind(state) == false || vmnl_net_socket_datagram_connect(sockets[0], &sockets.address(1), NULL) == false) {
        return;
    }
    for (auto _ : state) {
        for (size_t it = 0; it < DATAGRAM_BATCH; it++) {
            vmnl_net_socket_datagram_send(sockets[0], &view, 1, NULL);
        }
        state.PauseTiming();
        if (sockets.receive_from(state, 1, DATAGRAM_BATCH) == false) {
            break;
        }
        state.ResumeTiming();
    }
    state.counters["send"] = benchmark::Counter(DATAGRAM_BATCH, benchmark::Counter::kIsIterationInvariantRate | benchmark::Counter::kInvert);
}

BENCHMARK_CAPTURE(socket_datagram_send, udp_ipv4, udp_ipv4)->ArgName("bytes")->RangeMultiplier(8)->Range(64, DATAGRAM_SIZE);
BENCHMARK_CAPTURE(socket_datagram_send, udp_ipv6, udp_ipv6)->ArgName("bytes")->RangeMultiplier(8)->Range(64, DATAGRAM_SIZE);
