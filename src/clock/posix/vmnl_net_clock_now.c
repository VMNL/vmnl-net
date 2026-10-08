#include <vmnl/net/clock.h>

#include <time.h>

uint64_t vmnl_net_clock_now(void)
{
    struct timespec now;

#ifdef CLOCK_BOOTTIME
    clock_gettime(CLOCK_BOOTTIME, &now);
#else
    clock_gettime(CLOCK_MONOTONIC, &now);
#endif
    return (uint64_t) now.tv_sec * VMNL_NET_NANOSECONDS_PER_SECOND + (uint64_t) now.tv_nsec;
}
