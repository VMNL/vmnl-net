#include "timer_tests.h"

#include "utest.h"

static void set_interval_to_zero(VmnlNetTimer *timer, uint64_t lateness, void *data)
{
    count_calls(timer, lateness, data);
    vmnl_net_timer_set_interval(timer, 0);
}

TIMER_FIXTURE(vmnl_net_timer_set_interval)

UTEST_F(vmnl_net_timer_set_interval, rearms_timer_at_interval)
{
    vmnl_net_timer_set_interval(utest_fixture->timer, TIMEOUT / 2);
    vmnl_net_timer_start(utest_fixture->context, utest_fixture->timer, TIMEOUT);
    vmnl_net_context_advance(utest_fixture->context, START_TIME + TIMEOUT);
    ASSERT_EQ(utest_fixture->calls.count, 1);
    EXPECT_EQ(vmnl_net_timer_remaining(utest_fixture->timer, START_TIME + TIMEOUT), TIMEOUT / 2);
}

UTEST_F(vmnl_net_timer_set_interval, keeps_current_deadline)
{
    vmnl_net_timer_start(utest_fixture->context, utest_fixture->timer, TIMEOUT);
    vmnl_net_timer_set_interval(utest_fixture->timer, TIMEOUT / 2);
    EXPECT_EQ(vmnl_net_timer_remaining(utest_fixture->timer, START_TIME), TIMEOUT);
}

UTEST_F(vmnl_net_timer_set_interval, makes_timer_one_shot_with_zero_interval)
{
    vmnl_net_timer_set_interval(utest_fixture->timer, TIMEOUT);
    vmnl_net_timer_start(utest_fixture->context, utest_fixture->timer, TIMEOUT);
    vmnl_net_timer_set_interval(utest_fixture->timer, 0);
    vmnl_net_context_advance(utest_fixture->context, START_TIME + TIMEOUT);
    ASSERT_EQ(utest_fixture->calls.count, 1);
    EXPECT_FALSE(vmnl_net_timer_is_active(utest_fixture->timer));
}

UTEST_F(vmnl_net_timer_set_interval, makes_timer_one_shot_with_zero_interval_in_callback)
{
    vmnl_net_timer_set_interval(utest_fixture->timer, TIMEOUT);
    vmnl_net_timer_set_callback(utest_fixture->timer, set_interval_to_zero, &utest_fixture->calls);
    vmnl_net_timer_start(utest_fixture->context, utest_fixture->timer, TIMEOUT);
    vmnl_net_context_advance(utest_fixture->context, START_TIME + TIMEOUT);
    ASSERT_EQ(utest_fixture->calls.count, 1);
    EXPECT_FALSE(vmnl_net_timer_is_active(utest_fixture->timer));
}
