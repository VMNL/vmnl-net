#include "timer/timer_internal.h"
#include "context/context_internal.h"

void vmnl_net_timer_start(VmnlNetContext *context, VmnlNetTimer *timer, uint64_t timeout)
{
    vmnl_net_list_remove(&timer->link);
    timer->deadline = vmnl_net_timer_compute_deadline(context->timers.now, timeout);
    vmnl_net_timer_insert(&context->timers.active, timer);
}
