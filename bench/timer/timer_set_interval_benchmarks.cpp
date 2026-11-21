#include "timer_benchmarks.hpp"

static void timer_set_interval(benchmark::State &state)
{
    Timers timers(state, 1);

    for (auto _ : state) {
        vmnl_net_timer_set_interval(timers[0], 500 * MILLISECONDS);
    }
}

BENCHMARK(timer_set_interval);
