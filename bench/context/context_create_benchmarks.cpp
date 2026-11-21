#include <vmnl/net/context.h>

#include <benchmark/benchmark.h>

#include <vector>

#define CONTEXT_BATCH 1000 /*!< Contexts created per iteration. */

static void context_create(benchmark::State &state)
{
    std::vector<VmnlNetContext *> contexts(CONTEXT_BATCH);

    for (auto _ : state) {
        for (size_t it = 0; it < contexts.size(); it++) {
            contexts[it] = vmnl_net_context_create(NULL);
        }
        state.PauseTiming();
        for (const auto &context : contexts) {
            vmnl_net_context_destroy(context);
        }
        state.ResumeTiming();
    }
    state.counters["create"] = benchmark::Counter(CONTEXT_BATCH, benchmark::Counter::kIsIterationInvariantRate | benchmark::Counter::kInvert);
}

BENCHMARK(context_create);
