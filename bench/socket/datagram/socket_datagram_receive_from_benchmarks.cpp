#include "socket_datagram_benchmarks.hpp"

static void socket_datagram_receive_from(benchmark::State &state, const DatagramType &type)
{
    Sockets sockets(state, type, 2);
    std::vector<uint8_t> bytes(DATAGRAM_SIZE);
    VmnlNetBufferView view = {};
    VmnlNetAddress address;
    VmnlNetError error;

    view.data = bytes.data();
    view.size = (VmnlNetBufferViewSize) bytes.size();
    if (sockets.bind(state) == false) {
        return;
    }
    for (auto _ : state) {
        state.PauseTiming();
        if (sockets.send_to(state, 0, sockets.address(1), (size_t) state.range(0), DATAGRAM_BATCH) == false) {
            break;
        }
        state.ResumeTiming();
        for (size_t it = 0; it < DATAGRAM_BATCH; it++) {
            do {
                vmnl_net_socket_datagram_receive_from(sockets[1], &view, 1, &address, &error);
            } while (error == VMNL_NET_EAGAIN);
        }
    }
    state.counters["receive_from"] = benchmark::Counter(DATAGRAM_BATCH, benchmark::Counter::kIsIterationInvariantRate | benchmark::Counter::kInvert);
}

BENCHMARK_CAPTURE(socket_datagram_receive_from, udp_ipv4, udp_ipv4)->ArgName("bytes")->RangeMultiplier(8)->Range(64, DATAGRAM_SIZE);
BENCHMARK_CAPTURE(socket_datagram_receive_from, udp_ipv6, udp_ipv6)->ArgName("bytes")->RangeMultiplier(8)->Range(64, DATAGRAM_SIZE);
