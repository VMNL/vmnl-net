#include <vmnl/net/clock.h>

#include "utest.h"

UTEST(vmnl_net_clock_now, ticks_forward)
{
    const size_t nseconds = 3;
    uint64_t previous = vmnl_net_clock_now();
    uint64_t start = previous;
    uint64_t now;

    for (size_t it = 0; it < nseconds; it++) {
#ifdef _WIN32
        Sleep(1000);
#else
        sleep(1);
#endif
        now = vmnl_net_clock_now();
        EXPECT_GE(now, previous);
        previous = now;
    }
    EXPECT_GE(vmnl_net_clock_now() - start, nseconds * VMNL_NET_NANOSECONDS_PER_SECOND);
}

UTEST(vmnl_net_clock_now, advances_by_slept_time)
{
    const uint64_t previous = vmnl_net_clock_now();

#ifdef _WIN32
    Sleep(1000);
#else
    sleep(1);
#endif
    EXPECT_GE(vmnl_net_clock_now() - previous, VMNL_NET_NANOSECONDS_PER_SECOND);
}
