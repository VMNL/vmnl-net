#include "buffer_benchmarks.hpp"

static void buffer_create(benchmark::State &state, VmnlNetBufferType type)
{
    std::vector<VmnlNetBuffer *> buffers(BUFFER_BATCH);

    for (auto _ : state) {
        for (size_t it = 0; it < buffers.size(); it++) {
            buffers[it] = vmnl_net_buffer_create(type, (size_t) state.range(0), NULL);
        }
        state.PauseTiming();
        for (const auto &buffer : buffers) {
            vmnl_net_buffer_destroy(buffer);
        }
        state.ResumeTiming();
    }
    state.counters["create"] = benchmark::Counter(BUFFER_BATCH, benchmark::Counter::kIsIterationInvariantRate | benchmark::Counter::kInvert);
}

BENCHMARK_CAPTURE(buffer_create, flat, VMNL_NET_BUFFER_FLAT)->ArgName("capacity")->RangeMultiplier(16)->Range(64, BUFFER_CAPACITY);
BENCHMARK_CAPTURE(buffer_create, ring, VMNL_NET_BUFFER_RING)->ArgName("capacity")->RangeMultiplier(16)->Range(64, BUFFER_CAPACITY);
BENCHMARK_CAPTURE(buffer_create, dynamic, VMNL_NET_BUFFER_DYNAMIC)->ArgName("capacity")->RangeMultiplier(16)->Range(64, BUFFER_CAPACITY);
BENCHMARK_CAPTURE(buffer_create, chunked, VMNL_NET_BUFFER_CHUNKED)->ArgName("capacity")->RangeMultiplier(16)->Range(64, BUFFER_CAPACITY);
