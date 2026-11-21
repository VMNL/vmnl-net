#include "buffer_benchmarks.hpp"

static void buffer_cycle(benchmark::State &state, VmnlNetBufferType type)
{
    size_t nbytes = (size_t) state.range(0);
    Buffers buffers(state, type, BUFFER_CAPACITY, 1);
    std::vector<VmnlNetBufferView> views(BUFFER_VIEWS);

    for (auto _ : state) {
        size_t nviews = views.size();

        if (vmnl_net_buffer_prepare(buffers[0], nbytes, views.data(), &nviews, NULL) == false) {
            state.SkipWithError("vmnl_net_buffer_prepare() failed");
            break;
        }
        vmnl_net_buffer_commit(buffers[0], nbytes, NULL);
        nviews = views.size();
        benchmark::DoNotOptimize(vmnl_net_buffer_data(buffers[0], 0, views.data(), &nviews));
        vmnl_net_buffer_consume(buffers[0], nbytes, NULL);
    }
}

BENCHMARK_CAPTURE(buffer_cycle, flat, VMNL_NET_BUFFER_FLAT)->ArgName("bytes")->RangeMultiplier(16)->Range(64, BUFFER_CAPACITY);
BENCHMARK_CAPTURE(buffer_cycle, ring, VMNL_NET_BUFFER_RING)->ArgName("bytes")->RangeMultiplier(16)->Range(64, BUFFER_CAPACITY);
BENCHMARK_CAPTURE(buffer_cycle, dynamic, VMNL_NET_BUFFER_DYNAMIC)->ArgName("bytes")->RangeMultiplier(16)->Range(64, BUFFER_CAPACITY);
BENCHMARK_CAPTURE(buffer_cycle, chunked, VMNL_NET_BUFFER_CHUNKED)->ArgName("bytes")->RangeMultiplier(16)->Range(64, BUFFER_CAPACITY);
