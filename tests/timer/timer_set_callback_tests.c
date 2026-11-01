#include "timer_tests.h"

#include "utest.h"

TIMER_FIXTURE(vmnl_net_timer_set_callback)

UTEST_F(vmnl_net_timer_set_callback, passes_new_data)
{
    struct Calls calls = {0};

    vmnl_net_timer_set_callback(utest_fixture->timer, count_calls, &calls);
    vmnl_net_timer_start(utest_fixture->context, utest_fixture->timer, TIMEOUT);
    vmnl_net_context_advance(utest_fixture->context, START_TIME + TIMEOUT);
    EXPECT_EQ(calls.count, 1);
    EXPECT_EQ(utest_fixture->calls.count, 0);
}

UTEST_F(vmnl_net_timer_set_callback, accepts_null_data)
{
    null_data_calls = 0;
    vmnl_net_timer_set_callback(utest_fixture->timer, count_null_data, NULL);
    vmnl_net_timer_start(utest_fixture->context, utest_fixture->timer, TIMEOUT);
    vmnl_net_context_advance(utest_fixture->context, START_TIME + TIMEOUT);
    EXPECT_EQ(null_data_calls, 1);
}
