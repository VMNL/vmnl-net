#include "timer_tests.h"

#include "utest.h"

TIMER_FIXTURE(vmnl_net_timer_interval)

UTEST_F(vmnl_net_timer_interval, returns_zero_after_create)
{
    EXPECT_EQ(vmnl_net_timer_interval(utest_fixture->timer), UINT64_C(0));
}

UTEST_F(vmnl_net_timer_interval, returns_interval_set)
{
    vmnl_net_timer_set_interval(utest_fixture->timer, TIMEOUT);
    EXPECT_EQ(vmnl_net_timer_interval(utest_fixture->timer), TIMEOUT);
}
