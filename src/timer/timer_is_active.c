#include "timer/timer_internal.h"

bool vmnl_net_timer_is_active(const VmnlNetTimer *timer)
{
    return vmnl_net_list_is_empty(&timer->link) == false;
}
