#include "timer/timer_internal.h"

bool vmnl_net_timer_stop(VmnlNetTimer *timer)
{
    bool was_active = vmnl_net_timer_is_active(timer);

    vmnl_net_list_remove(&timer->link);
    return was_active;
}
