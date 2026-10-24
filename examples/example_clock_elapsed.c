#include <vmnl/net.h>

//! [elapsed]
static uint64_t measure(void)
{
    uint64_t start = vmnl_net_clock_now();

    // Work to measure
    return vmnl_net_clock_now() - start; // In nanoseconds, never negative since the clock never goes back
}
//! [elapsed]

//! [timeout]
static bool timed_out(uint64_t last_packet)
{
    // True when no packet arrived for the last 5 seconds
    return vmnl_net_clock_now() - last_packet > 5 * VMNL_NET_NANOSECONDS_PER_SECOND;
}
//! [timeout]

int main(void)
{
    uint64_t last_packet = vmnl_net_clock_now();

    measure();
    return (timed_out(last_packet) == true) ? 1 : 0;
}
