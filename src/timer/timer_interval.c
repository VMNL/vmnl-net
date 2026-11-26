#include "timer/timer_internal.h"

uint64_t vmnl_net_timer_interval(const VmnlNetTimer *timer)
{
    return timer->interval;
}
