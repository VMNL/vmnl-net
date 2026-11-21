/**
 * \file timer_benchmarks.hpp
 * \brief Context, timers, timeouts and callback shared by the timer benchmarks.
 */

#ifndef VMNL_NET_TIMER_BENCHMARKS_HPP
#define VMNL_NET_TIMER_BENCHMARKS_HPP

#include <vmnl/net/context.h>
#include <vmnl/net/timer.h>

#include <benchmark/benchmark.h>

#include <algorithm>
#include <cstdint>
#include <random>
#include <vector>

#define TIMER_SEED   UINT64_C(42) /*!< Seed of the timeouts, so that every run draws the same ones. */
#define TIMER_BATCH  1000 /*!< Timers started or stopped per iteration, the starts and stops of a frame in the hard problem. */
#define MILLISECONDS UINT64_C(1000000) /*!< Nanoseconds in a millisecond. */

/**
 * \brief Order of the deadlines of a benchmark.
 */
enum class Deadlines {
    in_order, /*!< 500 ms for every timer, so that each new deadline is the latest and ends the list, the best case of the sorted list. */
    random, /*!< Spread at random over 100 ms to 1 s. */
    backoff, /*!< 100 ms to 300 ms doubled 0 to 4 times, like a retransmission timeout after losses. */
};

/**
 * \brief Context of a benchmark, destroyed with the object.
 */
class Context {
  public:
    /**
     * \brief Creates the context.
     * \param state State of the benchmark, skipped with an error if the creation fails.
     */
    explicit Context(benchmark::State &state) : context(vmnl_net_context_create(NULL))
    {
        if (context == NULL) {
            state.SkipWithError("vmnl_net_context_create() failed");
        }
    }

    /**
     * \brief Destroys the context.
     */
    ~Context()
    {
        vmnl_net_context_destroy(context);
    }

    /**
     * \name Copies and moves
     * Not copyable nor movable, as two objects would destroy the context twice.
     * \{
     */
    Context(const Context &) = delete;
    Context &operator=(const Context &) = delete;
    Context(Context &&) = delete;
    Context &operator=(Context &&) = delete;
    /** \} */

    /**
     * \brief Gets the context.
     * \return Context, NULL if its creation failed.
     */
    VmnlNetContext *get() const
    {
        return context;
    }

  private:
    VmnlNetContext *context; /*!< Created context. */
};

/**
 * \brief Timers of a benchmark, destroyed with the object.
 */
class Timers {
  public:
    /**
     * \brief Callback of the timers, which does nothing.
     * \param timer Expired timer.
     * \param lateness Nanoseconds since the deadline.
     * \param data Unused.
     */
    static void ignore_call(VmnlNetTimer *timer, uint64_t lateness, void *data)
    {
        (void) timer;
        (void) lateness;
        (void) data;
    }

    /**
     * \brief Creates inactive one-shot timers that call ignore_call().
     * \param state State of the benchmark, skipped with an error if a creation fails.
     * \param ntimers Number of timers to create.
     */
    Timers(benchmark::State &state, size_t ntimers)
    {
        for (size_t it = 0; it < ntimers; it++) {
            VmnlNetTimer *timer = vmnl_net_timer_create(ignore_call, NULL, NULL);

            if (timer == NULL) {
                state.SkipWithError("vmnl_net_timer_create() failed");
                break;
            }
            timers.push_back(timer);
        }
    }

    /**
     * \brief Destroys the timers.
     */
    ~Timers()
    {
        for (const auto &timer : timers) {
            vmnl_net_timer_destroy(timer);
        }
    }

    /**
     * \name Copies and moves
     * Not copyable nor movable, as two objects would destroy the timers twice.
     * \{
     */
    Timers(const Timers &) = delete;
    Timers &operator=(const Timers &) = delete;
    Timers(Timers &&) = delete;
    Timers &operator=(Timers &&) = delete;
    /** \} */

    /**
     * \brief Starts the timers.
     * \param context Context of the timers.
     * \param timeouts Timeout of each timer, at least as many as the timers.
     */
    void start(VmnlNetContext *context, const std::vector<uint64_t> &timeouts) const
    {
        for (size_t it = 0; it < timers.size(); it++) {
            vmnl_net_timer_start(context, timers[it], timeouts[it]);
        }
    }

    /**
     * \brief Stops the timers.
     */
    void stop() const
    {
        for (const auto &timer : timers) {
            vmnl_net_timer_stop(timer);
        }
    }

    /**
     * \brief Gets a timer.
     * \param index Index of the timer, lower than size().
     * \return Timer at \p index.
     */
    VmnlNetTimer *operator[](size_t index) const
    {
        return timers[index];
    }

    /**
     * \brief Counts the timers.
     * \return Number of created timers, fewer than asked if a creation failed.
     */
    size_t size() const
    {
        return timers.size();
    }

  private:
    std::vector<VmnlNetTimer *> timers; /*!< Created timers. */
};

/**
 * \brief Context and timers of a benchmark, destroyed with the object, the timers before their context.
 */
struct TimerBench {
    Context context; /*!< Context of the timers. */
    Timers active; /*!< Timers started before the measure, and left active. */
    Timers batch; /*!< #TIMER_BATCH timers that the measure starts or stops. */
    std::vector<uint64_t> timeouts; /*!< Timeouts of the batch, drawn once. */

    /**
     * \brief Creates the context and the timers, then starts the active ones in the order of their deadlines, so that each start ends the list.
     * \param state State of the benchmark, skipped with an error if a creation fails.
     * \param nactive Number of timers to leave active.
     * \param deadlines Order of the deadlines of the active timers and of the batch.
     */
    TimerBench(benchmark::State &state, size_t nactive, Deadlines deadlines) : context(state), active(state, nactive), batch(state, TIMER_BATCH)
    {
        std::mt19937_64 generator(TIMER_SEED);
        std::vector<uint64_t> active_timeouts = draw_timeouts(generator, deadlines, nactive);

        std::sort(active_timeouts.begin(), active_timeouts.end());
        timeouts = draw_timeouts(generator, deadlines, TIMER_BATCH);
        if (context.get() != NULL && active.size() == nactive) {
            active.start(context.get(), active_timeouts);
        }
    }

  private:
    /**
     * \brief Draws timeouts.
     * \param generator Generator seeded with #TIMER_SEED.
     * \param deadlines Order of the deadlines to draw.
     * \param ntimeouts Number of timeouts to draw.
     * \return Timeouts in nanoseconds, in the order they were drawn.
     */
    static std::vector<uint64_t> draw_timeouts(std::mt19937_64 &generator, Deadlines deadlines, size_t ntimeouts)
    {
        std::uniform_int_distribution<uint64_t> spread(100 * MILLISECONDS, 1000 * MILLISECONDS);
        std::uniform_int_distribution<uint64_t> base(100 * MILLISECONDS, 300 * MILLISECONDS);
        std::uniform_int_distribution<unsigned> doublings(0, 4);
        std::vector<uint64_t> drawn(ntimeouts);

        for (size_t it = 0; it < ntimeouts; it++) {
            if (deadlines == Deadlines::in_order) {
                drawn[it] = 500 * MILLISECONDS;
            } else if (deadlines == Deadlines::random) {
                drawn[it] = spread(generator);
            } else {
                drawn[it] = base(generator) << doublings(generator);
            }
        }
        return drawn;
    }
};

#endif
