#include "timer_benchmarks.hpp"

static void timer_set_callback(benchmark::State &state)
{
    Timers timers(state, 1);

    for (auto _ : state) {
        vmnl_net_timer_set_callback(timers[0], Timers::ignore_call, NULL);
    }
}

BENCHMARK(timer_set_callback);
