#include "timer/timer_internal.h"

void vmnl_net_timer_set_interval(VmnlNetTimer *timer, uint64_t interval)
{
    timer->interval = interval;
}
