#include "timer_tests.h"

#include "utest.h"

struct StopCalls {
    int count;
    int stopped;
};

static void stop_itself(VmnlNetTimer *timer, uint64_t lateness, void *data)
{
    struct StopCalls *calls = data;

    (void) lateness;
    calls->count++;
    if (vmnl_net_timer_stop(timer) == true) {
        calls->stopped++;
    }
}

TIMER_FIXTURE(vmnl_net_timer_stop)

UTEST_F(vmnl_net_timer_stop, skips_stopped_timer)
{
    vmnl_net_timer_start(utest_fixture->context, utest_fixture->timer, TIMEOUT);
    ASSERT_TRUE(vmnl_net_timer_stop(utest_fixture->timer));
    vmnl_net_context_advance(utest_fixture->context, START_TIME + TIMEOUT);
    EXPECT_EQ(utest_fixture->calls.count, 0);
}

UTEST_F(vmnl_net_timer_stop, returns_true_when_active)
{
    vmnl_net_timer_start(utest_fixture->context, utest_fixture->timer, TIMEOUT);
    EXPECT_TRUE(vmnl_net_timer_stop(utest_fixture->timer));
}

UTEST_F(vmnl_net_timer_stop, returns_true_in_periodic_callback)
{
    struct StopCalls calls = {0};

    vmnl_net_timer_set_interval(utest_fixture->timer, TIMEOUT);
    vmnl_net_timer_set_callback(utest_fixture->timer, stop_itself, &calls);
    vmnl_net_timer_start(utest_fixture->context, utest_fixture->timer, TIMEOUT);
    vmnl_net_context_advance(utest_fixture->context, START_TIME + TIMEOUT);
    ASSERT_EQ(calls.count, 1);
    EXPECT_EQ(calls.stopped, 1);
}

UTEST_F(vmnl_net_timer_stop, returns_false_when_inactive)
{
    EXPECT_FALSE(vmnl_net_timer_stop(utest_fixture->timer));
}

UTEST_F(vmnl_net_timer_stop, returns_false_after_one_shot_expiration)
{
    vmnl_net_timer_start(utest_fixture->context, utest_fixture->timer, TIMEOUT);
    vmnl_net_context_advance(utest_fixture->context, START_TIME + TIMEOUT);
    ASSERT_EQ(utest_fixture->calls.count, 1);
    EXPECT_FALSE(vmnl_net_timer_stop(utest_fixture->timer));
}

UTEST_F(vmnl_net_timer_stop, returns_false_in_one_shot_callback)
{
    struct StopCalls calls = {0};

    vmnl_net_timer_set_callback(utest_fixture->timer, stop_itself, &calls);
    vmnl_net_timer_start(utest_fixture->context, utest_fixture->timer, TIMEOUT);
    vmnl_net_context_advance(utest_fixture->context, START_TIME + TIMEOUT);
    ASSERT_EQ(calls.count, 1);
    EXPECT_EQ(calls.stopped, 0);
}
