#include <vmnl/net/clock.h>

#include <windows.h>

uint64_t vmnl_net_clock_now(void)
{
    LARGE_INTEGER frequency;
    LARGE_INTEGER ticks;

    QueryPerformanceFrequency(&frequency);
    QueryPerformanceCounter(&ticks);
    return (uint64_t) (ticks.QuadPart / frequency.QuadPart) * VMNL_NET_NANOSECONDS_PER_SECOND + (uint64_t) (ticks.QuadPart % frequency.QuadPart) * VMNL_NET_NANOSECONDS_PER_SECOND / (uint64_t) frequency.QuadPart;
}
