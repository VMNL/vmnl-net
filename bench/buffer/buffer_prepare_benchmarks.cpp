#include "buffer_benchmarks.hpp"

static void buffer_prepare(benchmark::State &state, VmnlNetBufferType type)
{
    Buffers buffers(state, type, BUFFER_CAPACITY, 1);
    std::vector<VmnlNetBufferView> views(BUFFER_VIEWS);

    for (auto _ : state) {
        size_t nviews = views.size();

        benchmark::DoNotOptimize(vmnl_net_buffer_prepare(buffers[0], (size_t) state.range(0), views.data(), &nviews, NULL));
    }
}

BENCHMARK_CAPTURE(buffer_prepare, flat, VMNL_NET_BUFFER_FLAT)->ArgName("bytes")->RangeMultiplier(16)->Range(64, BUFFER_CAPACITY);
BENCHMARK_CAPTURE(buffer_prepare, ring, VMNL_NET_BUFFER_RING)->ArgName("bytes")->RangeMultiplier(16)->Range(64, BUFFER_CAPACITY);
BENCHMARK_CAPTURE(buffer_prepare, dynamic, VMNL_NET_BUFFER_DYNAMIC)->ArgName("bytes")->RangeMultiplier(16)->Range(64, BUFFER_CAPACITY);
BENCHMARK_CAPTURE(buffer_prepare, chunked, VMNL_NET_BUFFER_CHUNKED)->ArgName("bytes")->RangeMultiplier(16)->Range(64, BUFFER_CAPACITY);
