#include "timer/timer_internal.h"
#include "allocator/allocator_internal.h"

void vmnl_net_timer_destroy(VmnlNetTimer *timer)
{
    if (timer == NULL) {
        return;
    }
    vmnl_net_list_remove(&timer->link);
    vmnl_net_allocator.deallocate(timer);
}
