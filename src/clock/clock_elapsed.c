#include <vmnl/net/clock.h>

uint64_t vmnl_net_clock_elapsed(uint64_t start)
{
    uint64_t now = vmnl_net_clock_now();

    return (now > start) ? now - start : 0;
}
