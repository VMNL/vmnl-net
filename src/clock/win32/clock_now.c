#include <vmnl/net/clock.h>

#include <windows.h>

static LARGE_INTEGER vmnl_net_clock_now_frequency;

static INIT_ONCE vmnl_net_clock_now_init_once = INIT_ONCE_STATIC_INIT;

static BOOL CALLBACK vmnl_net_clock_query_performance_frequency(PINIT_ONCE once, PVOID parameter, PVOID *context)
{
    (void) once;
    (void) parameter;
    (void) context;
    return QueryPerformanceFrequency(&vmnl_net_clock_now_frequency);
}

uint64_t vmnl_net_clock_now(void)
{
    LARGE_INTEGER ticks;
    LARGE_INTEGER frequency;
    uint64_t whole_seconds;
    uint64_t remaining_ticks;

    InitOnceExecuteOnce(&vmnl_net_clock_now_init_once, vmnl_net_clock_query_performance_frequency, NULL, NULL);
    QueryPerformanceCounter(&ticks);
    frequency = vmnl_net_clock_now_frequency;
    whole_seconds = (uint64_t) ticks.QuadPart / frequency.QuadPart;
    remaining_ticks = (uint64_t) ticks.QuadPart % frequency.QuadPart;
    return whole_seconds * VMNL_NET_NANOSECONDS_PER_SECOND + remaining_ticks * VMNL_NET_NANOSECONDS_PER_SECOND / frequency.QuadPart;
}
