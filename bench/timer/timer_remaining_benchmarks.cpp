#include "timer_benchmarks.hpp"

static void timer_remaining(benchmark::State &state)
{
    TimerBench bench(state, 1, Deadlines::in_order);

    for (auto _ : state) {
        benchmark::DoNotOptimize(vmnl_net_timer_remaining(bench.active[0], 0));
    }
}

BENCHMARK(timer_remaining);
