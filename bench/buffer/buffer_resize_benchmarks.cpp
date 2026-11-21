#include "buffer_benchmarks.hpp"

static void buffer_resize(benchmark::State &state, VmnlNetBufferType type)
{
    size_t capacity = (size_t) state.range(0);
    Buffers buffers(state, type, capacity, BUFFER_BATCH);

    if (buffers.commit(state, capacity) == false) {
        return;
    }
    for (auto _ : state) {
        for (size_t it = 0; it < buffers.size(); it++) {
            vmnl_net_buffer_resize(buffers[it], 2 * capacity, NULL);
        }
        state.PauseTiming();
        for (size_t it = 0; it < buffers.size(); it++) {
            vmnl_net_buffer_shrink(buffers[it], capacity, NULL);
        }
        state.ResumeTiming();
    }
    state.counters["resize"] = benchmark::Counter(BUFFER_BATCH, benchmark::Counter::kIsIterationInvariantRate | benchmark::Counter::kInvert);
}

BENCHMARK_CAPTURE(buffer_resize, ring, VMNL_NET_BUFFER_RING)->ArgName("capacity")->RangeMultiplier(4)->Range(16384, BUFFER_CAPACITY);
BENCHMARK_CAPTURE(buffer_resize, dynamic, VMNL_NET_BUFFER_DYNAMIC)->ArgName("capacity")->RangeMultiplier(4)->Range(16384, BUFFER_CAPACITY);
BENCHMARK_CAPTURE(buffer_resize, chunked, VMNL_NET_BUFFER_CHUNKED)->ArgName("capacity")->RangeMultiplier(4)->Range(16384, BUFFER_CAPACITY);
