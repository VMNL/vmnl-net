#include <vmnl/net/clock.h>

#include <windows.h>

/**
 * \brief Frequency of the performance counter, fixed at boot.
 */
static LARGE_INTEGER vmnl_net_clock_frequency;

/**
 * \brief Guards the single read of the counter frequency.
 */
static INIT_ONCE vmnl_net_clock_init_once = INIT_ONCE_STATIC_INIT;

/**
 * \brief Reads the frequency of the performance counter.
 */
static BOOL CALLBACK vmnl_net_clock_get_frequency(PINIT_ONCE once, PVOID parameter, PVOID *context)
{
    (void) once;
    (void) parameter;
    (void) context;
    return QueryPerformanceFrequency(&vmnl_net_clock_frequency);
}

uint64_t vmnl_net_clock_now(void)
{
    LARGE_INTEGER ticks;

    InitOnceExecuteOnce(&vmnl_net_clock_init_once, vmnl_net_clock_get_frequency, NULL, NULL);
    QueryPerformanceCounter(&ticks);
    return (uint64_t) ticks.QuadPart / vmnl_net_clock_frequency.QuadPart * VMNL_NET_NANOSECONDS_PER_SECOND + (uint64_t) ticks.QuadPart % vmnl_net_clock_frequency.QuadPart * VMNL_NET_NANOSECONDS_PER_SECOND / vmnl_net_clock_frequency.QuadPart;
}
