#include <vmnl/net/clock.h>

#include <gtest/gtest.h>

#include <chrono>
#include <thread>

TEST(VmnlNetClockNow, NeverGoesBackwards)
{
    uint64_t previous = vmnl_net_clock_now();
    uint64_t now;

    for (size_t it = 0; it < 1000; it++) {
        now = vmnl_net_clock_now();
        EXPECT_GE(now, previous);
        previous = now;
    }
}

TEST(VmnlNetClockNow, Advances)
{
    uint64_t start = vmnl_net_clock_now();

    std::this_thread::sleep_for(std::chrono::milliseconds(10));
    EXPECT_GE(vmnl_net_clock_now() - start, 10000000);
}
