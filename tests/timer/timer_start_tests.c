#include <vmnl/net/clock.h>

#include "timer_tests.h"

#include "utest.h"

TIMER_FIXTURE(vmnl_net_timer_start)

UTEST_F(vmnl_net_timer_start, expires_timer_at_timeout)
{
    vmnl_net_timer_start(utest_fixture->context, utest_fixture->timer, TIMEOUT);
    vmnl_net_context_advance(utest_fixture->context, START_TIME + TIMEOUT);
    EXPECT_EQ(utest_fixture->calls.count, 1);
}

UTEST_F(vmnl_net_timer_start, counts_timeout_from_last_advance)
{
    vmnl_net_context_advance(utest_fixture->context, START_TIME + 500);
    vmnl_net_timer_start(utest_fixture->context, utest_fixture->timer, TIMEOUT);
    EXPECT_EQ(vmnl_net_timer_remaining(utest_fixture->timer, START_TIME + 500), TIMEOUT);
}

UTEST(vmnl_net_timer_start, counts_timeout_from_context_creation_before_advance)
{
    struct Calls calls = {0};
    uint64_t before = vmnl_net_clock_now();
    VmnlNetContext *context = vmnl_net_context_create(NULL);
    uint64_t after = vmnl_net_clock_now();
    VmnlNetTimer *timer = vmnl_net_timer_create(count_calls, &calls, NULL);

    ASSERT_NE(context, NULL);
    ASSERT_NE(timer, NULL);
    vmnl_net_timer_start(context, timer, TIMEOUT);
    EXPECT_GE(vmnl_net_timer_remaining(timer, before), TIMEOUT);
    EXPECT_LE(vmnl_net_timer_remaining(timer, after), TIMEOUT);
    vmnl_net_timer_destroy(timer);
    vmnl_net_context_destroy(context);
}

UTEST_F(vmnl_net_timer_start, postpones_active_timer)
{
    vmnl_net_timer_start(utest_fixture->context, utest_fixture->timer, TIMEOUT);
    vmnl_net_context_advance(utest_fixture->context, START_TIME + TIMEOUT / 2);
    vmnl_net_timer_start(utest_fixture->context, utest_fixture->timer, TIMEOUT);
    vmnl_net_context_advance(utest_fixture->context, START_TIME + TIMEOUT);
    EXPECT_EQ(utest_fixture->calls.count, 0);
}

UTEST_F(vmnl_net_timer_start, moves_active_timer_to_other_context)
{
    VmnlNetContext *context = vmnl_net_context_create(NULL);

    ASSERT_NE(context, NULL);
    vmnl_net_timer_start(utest_fixture->context, utest_fixture->timer, TIMEOUT);
    vmnl_net_timer_start(context, utest_fixture->timer, TIMEOUT);
    EXPECT_EQ(vmnl_net_context_remaining(utest_fixture->context, START_TIME), UINT64_MAX);
    EXPECT_NE(vmnl_net_context_remaining(context, START_TIME), UINT64_MAX);
    vmnl_net_context_destroy(context);
}

UTEST_F(vmnl_net_timer_start, skips_timer_before_timeout)
{
    vmnl_net_timer_start(utest_fixture->context, utest_fixture->timer, TIMEOUT);
    vmnl_net_context_advance(utest_fixture->context, START_TIME + TIMEOUT - 1);
    EXPECT_EQ(utest_fixture->calls.count, 0);
}

UTEST_F(vmnl_net_timer_start, expires_timer_at_next_advance_with_zero_timeout)
{
    vmnl_net_timer_start(utest_fixture->context, utest_fixture->timer, 0);
    vmnl_net_context_advance(utest_fixture->context, START_TIME);
    EXPECT_EQ(utest_fixture->calls.count, 1);
}

UTEST_F(vmnl_net_timer_start, saturates_deadline_at_uint64_max)
{
    vmnl_net_timer_start(utest_fixture->context, utest_fixture->timer, UINT64_MAX);
    EXPECT_EQ(vmnl_net_timer_remaining(utest_fixture->timer, START_TIME), UINT64_MAX - START_TIME);
}
