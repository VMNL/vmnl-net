#include "timer/timer_internal.h"

void vmnl_net_timer_stop(VmnlNetContext *context, VmnlNetTimer *timer)
{
    if (context == NULL || timer == NULL) {
        return;
    }
    vmnl_net_timer_remove(timer);
}
