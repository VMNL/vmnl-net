#include "timer_benchmarks.hpp"

static void timer_interval(benchmark::State &state)
{
    Timers timers(state, 1);

    for (auto _ : state) {
        benchmark::DoNotOptimize(vmnl_net_timer_interval(timers[0]));
    }
}

BENCHMARK(timer_interval);
