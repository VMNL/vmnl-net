#include "timer/timer_internal.h"
#include "context/context_internal.h"

void vmnl_net_timer_destroy(VmnlNetContext *context, VmnlNetTimer *timer)
{
    if (context == NULL || timer == NULL) {
        return;
    }
    vmnl_net_timer_remove(timer);
    context->allocator.deallocate(timer);
}
