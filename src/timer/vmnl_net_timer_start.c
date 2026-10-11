#include "timer/timer_internal.h"

void vmnl_net_timer_start(VmnlNetContext *context, VmnlNetTimer *timer, uint64_t now, uint64_t timeout, uint64_t interval)
{
    if (context == NULL || timer == NULL) {
        return;
    }
    timer->timeout = timeout;
    timer->interval = interval;
    vmnl_net_timer_reset(context, timer, now);
}
