#include "timer/timer_internal.h"

void vmnl_net_timer_reset(VmnlNetContext *context, VmnlNetTimer *timer, uint64_t now)
{
    if (context == NULL || timer == NULL) {
        return;
    }
    vmnl_net_timer_remove(timer);
    timer->deadline = vmnl_net_timer_add(now, timer->timeout);
    vmnl_net_timer_insert(context, timer);
}
