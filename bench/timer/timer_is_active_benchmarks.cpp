#include "timer_benchmarks.hpp"

static void timer_is_active(benchmark::State &state)
{
    Timers timers(state, 1);

    for (auto _ : state) {
        benchmark::DoNotOptimize(vmnl_net_timer_is_active(timers[0]));
    }
}

BENCHMARK(timer_is_active);
