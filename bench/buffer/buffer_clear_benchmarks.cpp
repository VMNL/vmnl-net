#include "buffer_benchmarks.hpp"

static void buffer_clear(benchmark::State &state, VmnlNetBufferType type)
{
    Buffers buffers(state, type, BUFFER_CAPACITY, BUFFER_BATCH);

    for (auto _ : state) {
        state.PauseTiming();
        if (buffers.commit(state, (size_t) state.range(0)) == false) {
            break;
        }
        state.ResumeTiming();
        buffers.clear();
    }
    state.counters["clear"] = benchmark::Counter(BUFFER_BATCH, benchmark::Counter::kIsIterationInvariantRate | benchmark::Counter::kInvert);
}

BENCHMARK_CAPTURE(buffer_clear, flat, VMNL_NET_BUFFER_FLAT)->ArgName("bytes")->RangeMultiplier(16)->Range(64, BUFFER_CAPACITY);
BENCHMARK_CAPTURE(buffer_clear, ring, VMNL_NET_BUFFER_RING)->ArgName("bytes")->RangeMultiplier(16)->Range(64, BUFFER_CAPACITY);
BENCHMARK_CAPTURE(buffer_clear, dynamic, VMNL_NET_BUFFER_DYNAMIC)->ArgName("bytes")->RangeMultiplier(16)->Range(64, BUFFER_CAPACITY);
BENCHMARK_CAPTURE(buffer_clear, chunked, VMNL_NET_BUFFER_CHUNKED)->ArgName("bytes")->RangeMultiplier(16)->Range(64, BUFFER_CAPACITY);
