#include "../timer/timer_benchmarks.hpp"

static void context_remaining(benchmark::State &state)
{
    TimerBench bench(state, 1, Deadlines::in_order);

    for (auto _ : state) {
        benchmark::DoNotOptimize(vmnl_net_context_remaining(bench.context.get(), 0));
    }
}

BENCHMARK(context_remaining);
