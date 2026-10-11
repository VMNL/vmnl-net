#include "timer/timer_internal.h"

void vmnl_net_timer_remove(VmnlNetTimer *timer)
{
    vmnl_net_list_remove(&timer->node);
}
