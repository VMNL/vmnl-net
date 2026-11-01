#include "timer/timer_internal.h"

void vmnl_net_timer_set_callback(VmnlNetTimer *timer, VmnlNetTimerCallback callback, void *data)
{
    timer->callback = callback;
    timer->data = data;
}
