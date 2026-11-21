#include "timer_benchmarks.hpp"

static void timer_create(benchmark::State &state)
{
    std::vector<VmnlNetTimer *> timers(TIMER_BATCH);

    for (auto _ : state) {
        for (size_t it = 0; it < timers.size(); it++) {
            timers[it] = vmnl_net_timer_create(Timers::ignore_call, NULL, NULL);
        }
        state.PauseTiming();
        for (const auto &timer : timers) {
            vmnl_net_timer_destroy(timer);
        }
        state.ResumeTiming();
    }
    state.counters["create"] = benchmark::Counter(TIMER_BATCH, benchmark::Counter::kIsIterationInvariantRate | benchmark::Counter::kInvert);
}

BENCHMARK(timer_create);
