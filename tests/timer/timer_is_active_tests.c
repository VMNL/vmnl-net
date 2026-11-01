#include "timer_tests.h"

#include "utest.h"

struct ActiveCalls {
    int count;
    int active;
};

static void count_active_calls(VmnlNetTimer *timer, uint64_t lateness, void *data)
{
    struct ActiveCalls *calls = data;

    (void) lateness;
    calls->count++;
    if (vmnl_net_timer_is_active(timer) == true) {
        calls->active++;
    }
}

TIMER_FIXTURE(vmnl_net_timer_is_active)

UTEST_F(vmnl_net_timer_is_active, returns_true_after_start)
{
    vmnl_net_timer_start(utest_fixture->context, utest_fixture->timer, TIMEOUT);
    EXPECT_TRUE(vmnl_net_timer_is_active(utest_fixture->timer));
}

UTEST_F(vmnl_net_timer_is_active, returns_true_after_periodic_expiration)
{
    vmnl_net_timer_set_interval(utest_fixture->timer, TIMEOUT);
    vmnl_net_timer_start(utest_fixture->context, utest_fixture->timer, TIMEOUT);
    vmnl_net_context_advance(utest_fixture->context, START_TIME + TIMEOUT);
    ASSERT_EQ(utest_fixture->calls.count, 1);
    EXPECT_TRUE(vmnl_net_timer_is_active(utest_fixture->timer));
}

UTEST_F(vmnl_net_timer_is_active, returns_true_in_periodic_callback)
{
    struct ActiveCalls calls = {0};

    vmnl_net_timer_set_interval(utest_fixture->timer, TIMEOUT);
    vmnl_net_timer_set_callback(utest_fixture->timer, count_active_calls, &calls);
    vmnl_net_timer_start(utest_fixture->context, utest_fixture->timer, TIMEOUT);
    vmnl_net_context_advance(utest_fixture->context, START_TIME + TIMEOUT);
    ASSERT_EQ(calls.count, 1);
    EXPECT_EQ(calls.active, 1);
}

UTEST_F(vmnl_net_timer_is_active, returns_false_after_create)
{
    EXPECT_FALSE(vmnl_net_timer_is_active(utest_fixture->timer));
}

UTEST_F(vmnl_net_timer_is_active, returns_false_after_stop)
{
    vmnl_net_timer_start(utest_fixture->context, utest_fixture->timer, TIMEOUT);
    ASSERT_TRUE(vmnl_net_timer_stop(utest_fixture->timer));
    EXPECT_FALSE(vmnl_net_timer_is_active(utest_fixture->timer));
}

UTEST_F(vmnl_net_timer_is_active, returns_false_after_one_shot_expiration)
{
    vmnl_net_timer_start(utest_fixture->context, utest_fixture->timer, TIMEOUT);
    vmnl_net_context_advance(utest_fixture->context, START_TIME + TIMEOUT);
    ASSERT_EQ(utest_fixture->calls.count, 1);
    EXPECT_FALSE(vmnl_net_timer_is_active(utest_fixture->timer));
}

UTEST_F(vmnl_net_timer_is_active, returns_false_in_one_shot_callback)
{
    struct ActiveCalls calls = {0};

    vmnl_net_timer_set_callback(utest_fixture->timer, count_active_calls, &calls);
    vmnl_net_timer_start(utest_fixture->context, utest_fixture->timer, TIMEOUT);
    vmnl_net_context_advance(utest_fixture->context, START_TIME + TIMEOUT);
    ASSERT_EQ(calls.count, 1);
    EXPECT_EQ(calls.active, 0);
}
