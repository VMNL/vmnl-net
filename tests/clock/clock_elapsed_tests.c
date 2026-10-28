#include <vmnl/net/clock.h>

#include "utest.h"

UTEST(vmnl_net_clock_elapsed, returns_time_since_start)
{
    const uint64_t clock = vmnl_net_clock_now() - VMNL_NET_NANOSECONDS_PER_SECOND;
    const uint64_t elapsed = vmnl_net_clock_elapsed(clock);

    EXPECT_GE(elapsed, VMNL_NET_NANOSECONDS_PER_SECOND);
    EXPECT_LE(elapsed, vmnl_net_clock_now() - clock);
}

UTEST(vmnl_net_clock_elapsed, returns_zero_with_future_start)
{
    EXPECT_EQ(vmnl_net_clock_elapsed(vmnl_net_clock_now() + VMNL_NET_NANOSECONDS_PER_SECOND), UINT64_C(0));
}
