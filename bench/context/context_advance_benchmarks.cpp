#include "../timer/timer_benchmarks.hpp"

static void context_advance(benchmark::State &state)
{
    Context context(state);
    Timers expiring(state, (size_t) state.range(0));
    std::vector<uint64_t> timeouts(expiring.size(), 500 * MILLISECONDS);
    uint64_t now = 0;

    for (auto _ : state) {
        state.PauseTiming();
        expiring.start(context.get(), timeouts);
        now += 1000 * MILLISECONDS;
        state.ResumeTiming();
        vmnl_net_context_advance(context.get(), now);
    }
    state.counters["expiration"] = benchmark::Counter((double) expiring.size(), benchmark::Counter::kIsIterationInvariantRate | benchmark::Counter::kInvert);
}

BENCHMARK(context_advance)->ArgName("expiring")->Arg(1000)->Arg(10000)->Arg(32000);
