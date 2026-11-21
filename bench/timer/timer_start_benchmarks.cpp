#include "timer_benchmarks.hpp"

static void timer_start(benchmark::State &state, Deadlines deadlines)
{
    TimerBench bench(state, (size_t) state.range(0), deadlines);

    for (auto _ : state) {
        bench.batch.start(bench.context.get(), bench.timeouts);
        state.PauseTiming();
        bench.batch.stop();
        state.ResumeTiming();
    }
    state.counters["start"] = benchmark::Counter(TIMER_BATCH, benchmark::Counter::kIsIterationInvariantRate | benchmark::Counter::kInvert);
}

BENCHMARK_CAPTURE(timer_start, in_order, Deadlines::in_order)->ArgName("active")->Arg(1000)->Arg(10000)->Arg(32000)->Arg(100000);
BENCHMARK_CAPTURE(timer_start, random, Deadlines::random)->ArgName("active")->Arg(1000)->Arg(10000)->Arg(32000)->Arg(100000);
BENCHMARK_CAPTURE(timer_start, backoff, Deadlines::backoff)->ArgName("active")->Arg(1000)->Arg(10000)->Arg(32000)->Arg(100000);
