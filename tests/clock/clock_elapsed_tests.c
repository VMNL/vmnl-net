#include <vmnl/net/clock.h>

#include "utest.h"

#include <stdint.h>

UTEST(vmnl_net_clock_elapsed, success)
{
    const uint64_t start = vmnl_net_clock_now() - VMNL_NET_NANOSECONDS_PER_SECOND;
    const uint64_t elapsed = vmnl_net_clock_elapsed(start);
    const uint64_t end = vmnl_net_clock_now();

    EXPECT_GE(elapsed, VMNL_NET_NANOSECONDS_PER_SECOND);
    EXPECT_LE(elapsed, end - start);
}

UTEST(vmnl_net_clock_elapsed, future_start)
{
    EXPECT_EQ(vmnl_net_clock_elapsed(vmnl_net_clock_now() + VMNL_NET_NANOSECONDS_PER_SECOND), UINT64_C(0));
}

UTEST(vmnl_net_clock_elapsed, largest_start)
{
    EXPECT_EQ(vmnl_net_clock_elapsed(UINT64_MAX), UINT64_C(0));
}
