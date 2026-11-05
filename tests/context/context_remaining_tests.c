#include "timer/timer_tests.h"

#include "utest.h"

TIMER_FIXTURE(vmnl_net_context_remaining)

UTEST_F(vmnl_net_context_remaining, returns_time_left_before_first_deadline)
{
    struct Calls calls = {0};
    VmnlNetTimer *timer = vmnl_net_timer_create(count_calls, &calls, NULL);

    ASSERT_NE(timer, NULL);
    vmnl_net_timer_start(utest_fixture->context, utest_fixture->timer, TIMEOUT);
    vmnl_net_timer_start(utest_fixture->context, timer, TIMEOUT / 2);
    EXPECT_EQ(vmnl_net_context_remaining(utest_fixture->context, START_TIME + 20), TIMEOUT / 2 - 20);
    vmnl_net_timer_destroy(timer);
}

UTEST_F(vmnl_net_context_remaining, returns_one_before_first_deadline)
{
    vmnl_net_timer_start(utest_fixture->context, utest_fixture->timer, TIMEOUT);
    EXPECT_EQ(vmnl_net_context_remaining(utest_fixture->context, START_TIME + TIMEOUT - 1), UINT64_C(1));
}

UTEST_F(vmnl_net_context_remaining, returns_zero_at_first_deadline)
{
    vmnl_net_timer_start(utest_fixture->context, utest_fixture->timer, TIMEOUT);
    EXPECT_EQ(vmnl_net_context_remaining(utest_fixture->context, START_TIME + TIMEOUT), UINT64_C(0));
}

UTEST_F(vmnl_net_context_remaining, returns_uint64_max_without_active_timer)
{
    EXPECT_EQ(vmnl_net_context_remaining(utest_fixture->context, START_TIME), UINT64_MAX);
}
