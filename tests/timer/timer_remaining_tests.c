#include "timer_tests.h"

#include "utest.h"

TIMER_FIXTURE(vmnl_net_timer_remaining)

UTEST_F(vmnl_net_timer_remaining, returns_time_left_before_deadline)
{
    vmnl_net_timer_start(utest_fixture->context, utest_fixture->timer, TIMEOUT);
    EXPECT_EQ(vmnl_net_timer_remaining(utest_fixture->timer, START_TIME + 30), TIMEOUT - 30);
}

UTEST_F(vmnl_net_timer_remaining, returns_time_left_before_next_periodic_deadline)
{
    vmnl_net_timer_set_interval(utest_fixture->timer, TIMEOUT);
    vmnl_net_timer_start(utest_fixture->context, utest_fixture->timer, TIMEOUT);
    vmnl_net_context_advance(utest_fixture->context, START_TIME + TIMEOUT + 30);
    ASSERT_EQ(utest_fixture->calls.count, 1);
    EXPECT_EQ(vmnl_net_timer_remaining(utest_fixture->timer, START_TIME + TIMEOUT + 30), TIMEOUT - 30);
}

UTEST_F(vmnl_net_timer_remaining, returns_one_before_deadline)
{
    vmnl_net_timer_start(utest_fixture->context, utest_fixture->timer, TIMEOUT);
    EXPECT_EQ(vmnl_net_timer_remaining(utest_fixture->timer, START_TIME + TIMEOUT - 1), UINT64_C(1));
}

UTEST_F(vmnl_net_timer_remaining, returns_zero_at_deadline)
{
    vmnl_net_timer_start(utest_fixture->context, utest_fixture->timer, TIMEOUT);
    EXPECT_EQ(vmnl_net_timer_remaining(utest_fixture->timer, START_TIME + TIMEOUT), UINT64_C(0));
}

UTEST_F(vmnl_net_timer_remaining, returns_zero_after_deadline)
{
    vmnl_net_timer_start(utest_fixture->context, utest_fixture->timer, TIMEOUT);
    EXPECT_EQ(vmnl_net_timer_remaining(utest_fixture->timer, START_TIME + TIMEOUT + 1), UINT64_C(0));
}
