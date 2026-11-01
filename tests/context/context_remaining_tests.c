#include "timer/timer_tests.h"

#include "utest.h"

TIMER_FIXTURE(vmnl_net_context_remaining)

UTEST_F(vmnl_net_context_remaining, returns_true_with_active_timer)
{
    uint64_t remaining;

    vmnl_net_timer_start(utest_fixture->context, utest_fixture->timer, TIMEOUT);
    EXPECT_TRUE(vmnl_net_context_remaining(utest_fixture->context, START_TIME, &remaining));
}

UTEST_F(vmnl_net_context_remaining, writes_time_left_before_first_deadline)
{
    struct Calls calls = {0};
    VmnlNetTimer *timer = vmnl_net_timer_create(count_calls, &calls, NULL);
    uint64_t remaining = 0;

    ASSERT_NE(timer, NULL);
    vmnl_net_timer_start(utest_fixture->context, utest_fixture->timer, TIMEOUT);
    vmnl_net_timer_start(utest_fixture->context, timer, TIMEOUT / 2);
    ASSERT_TRUE(vmnl_net_context_remaining(utest_fixture->context, START_TIME + 20, &remaining));
    EXPECT_EQ(remaining, TIMEOUT / 2 - 20);
    vmnl_net_timer_destroy(timer);
}

UTEST_F(vmnl_net_context_remaining, writes_one_before_first_deadline)
{
    uint64_t remaining = 0;

    vmnl_net_timer_start(utest_fixture->context, utest_fixture->timer, TIMEOUT);
    ASSERT_TRUE(vmnl_net_context_remaining(utest_fixture->context, START_TIME + TIMEOUT - 1, &remaining));
    EXPECT_EQ(remaining, UINT64_C(1));
}

UTEST_F(vmnl_net_context_remaining, writes_zero_at_first_deadline)
{
    uint64_t remaining = 1;

    vmnl_net_timer_start(utest_fixture->context, utest_fixture->timer, TIMEOUT);
    ASSERT_TRUE(vmnl_net_context_remaining(utest_fixture->context, START_TIME + TIMEOUT, &remaining));
    EXPECT_EQ(remaining, UINT64_C(0));
}

UTEST_F(vmnl_net_context_remaining, returns_false_without_active_timer)
{
    uint64_t remaining;

    EXPECT_FALSE(vmnl_net_context_remaining(utest_fixture->context, START_TIME, &remaining));
}

UTEST_F(vmnl_net_context_remaining, keeps_remaining_without_active_timer)
{
    uint64_t remaining = 42;

    ASSERT_FALSE(vmnl_net_context_remaining(utest_fixture->context, START_TIME, &remaining));
    EXPECT_EQ(remaining, UINT64_C(42));
}
