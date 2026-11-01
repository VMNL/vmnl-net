/**
 * \file timer_tests.h
 * \brief Fixture and callbacks shared by the timer tests.
 */

#ifndef VMNL_NET_TIMER_TESTS_H
#define VMNL_NET_TIMER_TESTS_H

#include <vmnl/net/allocator.h>
#include <vmnl/net/timer.h>

#include "utest.h"

#define START_TIME UINT64_C(1000) /*!< Time of the context at the start of a #TIMER_FIXTURE test. */
#define TIMEOUT    UINT64_C(100) /*!< Timeout of the timers of the tests. */

/**
 * \brief Declares the fixture of a UTEST_F suite: a context advanced to #START_TIME, and a timer whose calls count_calls() counts in calls.
 * \param SUITE Name of the suite.
 * \remark The teardown restores the default allocator, then destroys the timer and the context.
 */
#define TIMER_FIXTURE(SUITE)                                                                    \
    struct SUITE {                                                                              \
        VmnlNetContext *context;                                                                \
        VmnlNetTimer *timer;                                                                    \
        struct Calls calls;                                                                     \
    };                                                                                          \
                                                                                                \
    UTEST_F_SETUP(SUITE)                                                                        \
    {                                                                                           \
        utest_fixture->context = vmnl_net_context_create(NULL);                                 \
        ASSERT_NE(utest_fixture->context, NULL);                                                \
        utest_fixture->timer = vmnl_net_timer_create(count_calls, &utest_fixture->calls, NULL); \
        ASSERT_NE(utest_fixture->timer, NULL);                                                  \
        vmnl_net_context_advance(utest_fixture->context, START_TIME);                           \
    }                                                                                           \
                                                                                                \
    UTEST_F_TEARDOWN(SUITE)                                                                     \
    {                                                                                           \
        (void) utest_result;                                                                    \
        vmnl_net_allocator_set(malloc, realloc, free);                                          \
        vmnl_net_timer_destroy(utest_fixture->timer);                                           \
        vmnl_net_context_destroy(utest_fixture->context);                                       \
    }

/**
 * \brief Calls of a timer counted by count_calls().
 */
struct Calls {
    int count; /*!< Number of calls. */
    uint64_t lateness; /*!< Lateness of the last call. */
};

/**
 * \brief Calls of count_null_data() that received a NULL data, set to 0 by the tests that read it.
 */
static int null_data_calls;

/**
 * \brief Counts a call of a timer.
 * \param timer Expired timer, unused.
 * \param lateness Lateness of the call, stored in \p data.
 * \param data Calls of the timer, a struct Calls.
 */
static inline void count_calls(VmnlNetTimer *timer, uint64_t lateness, void *data)
{
    struct Calls *calls = data;

    (void) timer;
    calls->count++;
    calls->lateness = lateness;
}

/**
 * \brief Counts a call that received a NULL data.
 * \param timer Expired timer, unused.
 * \param lateness Lateness of the call, unused.
 * \param data Data of the timer, counted in #null_data_calls when NULL.
 */
static inline void count_null_data(VmnlNetTimer *timer, uint64_t lateness, void *data)
{
    (void) timer;
    (void) lateness;
    if (data == NULL) {
        null_data_calls++;
    }
}

#endif /* !VMNL_NET_TIMER_TESTS_H */
