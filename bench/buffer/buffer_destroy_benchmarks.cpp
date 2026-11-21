#include "buffer_benchmarks.hpp"

static void buffer_destroy(benchmark::State &state, VmnlNetBufferType type)
{
    std::vector<VmnlNetBuffer *> buffers(BUFFER_BATCH);

    for (auto _ : state) {
        state.PauseTiming();
        for (size_t it = 0; it < buffers.size(); it++) {
            buffers[it] = vmnl_net_buffer_create(type, (size_t) state.range(0), NULL);
        }
        state.ResumeTiming();
        for (const auto &buffer : buffers) {
            vmnl_net_buffer_destroy(buffer);
        }
    }
    state.counters["destroy"] = benchmark::Counter(BUFFER_BATCH, benchmark::Counter::kIsIterationInvariantRate | benchmark::Counter::kInvert);
}

BENCHMARK_CAPTURE(buffer_destroy, flat, VMNL_NET_BUFFER_FLAT)->ArgName("capacity")->RangeMultiplier(16)->Range(64, BUFFER_CAPACITY);
BENCHMARK_CAPTURE(buffer_destroy, ring, VMNL_NET_BUFFER_RING)->ArgName("capacity")->RangeMultiplier(16)->Range(64, BUFFER_CAPACITY);
BENCHMARK_CAPTURE(buffer_destroy, dynamic, VMNL_NET_BUFFER_DYNAMIC)->ArgName("capacity")->RangeMultiplier(16)->Range(64, BUFFER_CAPACITY);
BENCHMARK_CAPTURE(buffer_destroy, chunked, VMNL_NET_BUFFER_CHUNKED)->ArgName("capacity")->RangeMultiplier(16)->Range(64, BUFFER_CAPACITY);
