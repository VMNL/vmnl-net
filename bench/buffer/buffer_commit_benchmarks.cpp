#include "buffer_benchmarks.hpp"

static void buffer_commit(benchmark::State &state, VmnlNetBufferType type)
{
    size_t nbytes = (size_t) state.range(0);
    Buffers buffers(state, type, BUFFER_CAPACITY, BUFFER_BATCH);

    for (auto _ : state) {
        state.PauseTiming();
        buffers.clear();
        if (buffers.prepare(state, nbytes) == false) {
            break;
        }
        state.ResumeTiming();
        for (size_t it = 0; it < buffers.size(); it++) {
            vmnl_net_buffer_commit(buffers[it], nbytes, NULL);
        }
    }
    state.counters["commit"] = benchmark::Counter(BUFFER_BATCH, benchmark::Counter::kIsIterationInvariantRate | benchmark::Counter::kInvert);
}

BENCHMARK_CAPTURE(buffer_commit, flat, VMNL_NET_BUFFER_FLAT)->ArgName("bytes")->RangeMultiplier(16)->Range(64, BUFFER_CAPACITY);
BENCHMARK_CAPTURE(buffer_commit, ring, VMNL_NET_BUFFER_RING)->ArgName("bytes")->RangeMultiplier(16)->Range(64, BUFFER_CAPACITY);
BENCHMARK_CAPTURE(buffer_commit, dynamic, VMNL_NET_BUFFER_DYNAMIC)->ArgName("bytes")->RangeMultiplier(16)->Range(64, BUFFER_CAPACITY);
BENCHMARK_CAPTURE(buffer_commit, chunked, VMNL_NET_BUFFER_CHUNKED)->ArgName("bytes")->RangeMultiplier(16)->Range(64, BUFFER_CAPACITY);
