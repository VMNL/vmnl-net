#include "timer/timer_internal.h"

#include <stdint.h>

uint64_t vmnl_net_timer_remaining(const VmnlNetTimer *timer, uint64_t now)
{
    return (timer->deadline > now) ? timer->deadline - now : 0;
}
