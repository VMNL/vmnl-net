#include <vmnl/net/clock.h>

#include "utest.h"

#include <stddef.h>
#include <stdint.h>
#include <time.h>

UTEST(vmnl_net_clock_now, never_goes_backwards)
{
    uint64_t previous = vmnl_net_clock_now();
    uint64_t now;

    for (size_t it = 0; it < 1000; it++) {
        now = vmnl_net_clock_now();
        EXPECT_GE(now, previous);
        previous = now;
    }
}

UTEST(vmnl_net_clock_now, counts_nanoseconds)
{
    const uint64_t start = vmnl_net_clock_now();
    const clock_t time = clock();

    ASSERT_NE(time, (clock_t) -1);
    // clock() rounds down, one more tick covers it
    while (clock() - time < CLOCKS_PER_SEC / 100 + 1)
        ;
    EXPECT_GE(vmnl_net_clock_now() - start, VMNL_NET_NANOSECONDS_PER_SECOND / 100);
}
