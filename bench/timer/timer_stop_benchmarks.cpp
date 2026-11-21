#include "timer_benchmarks.hpp"

static void timer_stop(benchmark::State &state)
{
    TimerBench bench(state, (size_t) state.range(0), Deadlines::in_order);

    for (auto _ : state) {
        state.PauseTiming();
        bench.batch.start(bench.context.get(), bench.timeouts);
        state.ResumeTiming();
        bench.batch.stop();
    }
    state.counters["stop"] = benchmark::Counter(TIMER_BATCH, benchmark::Counter::kIsIterationInvariantRate | benchmark::Counter::kInvert);
}

BENCHMARK(timer_stop)->ArgName("active")->Arg(1000)->Arg(10000)->Arg(32000)->Arg(100000);
