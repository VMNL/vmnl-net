#include "buffer_benchmarks.hpp"

static void buffer_data(benchmark::State &state, VmnlNetBufferType type)
{
    Buffers buffers(state, type, BUFFER_CAPACITY, 1);
    std::vector<VmnlNetBufferView> views(BUFFER_VIEWS);

    if (buffers.commit(state, (size_t) state.range(0)) == false) {
        return;
    }
    for (auto _ : state) {
        size_t nviews = views.size();

        benchmark::DoNotOptimize(vmnl_net_buffer_data(buffers[0], 0, views.data(), &nviews));
    }
}

BENCHMARK_CAPTURE(buffer_data, flat, VMNL_NET_BUFFER_FLAT)->ArgName("bytes")->RangeMultiplier(16)->Range(64, BUFFER_CAPACITY);
BENCHMARK_CAPTURE(buffer_data, ring, VMNL_NET_BUFFER_RING)->ArgName("bytes")->RangeMultiplier(16)->Range(64, BUFFER_CAPACITY);
BENCHMARK_CAPTURE(buffer_data, dynamic, VMNL_NET_BUFFER_DYNAMIC)->ArgName("bytes")->RangeMultiplier(16)->Range(64, BUFFER_CAPACITY);
BENCHMARK_CAPTURE(buffer_data, chunked, VMNL_NET_BUFFER_CHUNKED)->ArgName("bytes")->RangeMultiplier(16)->Range(64, BUFFER_CAPACITY);
