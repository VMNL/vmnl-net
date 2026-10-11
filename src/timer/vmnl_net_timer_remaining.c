#include "timer/timer_internal.h"
#include "context/context_internal.h"

uint64_t vmnl_net_timer_remaining(const VmnlNetContext *context, uint64_t now)
{
    const VmnlNetTimer *timer;

    if (context == NULL || vmnl_net_list_empty(&context->timers) == true) {
        return UINT64_MAX;
    }
    timer = vmnl_net_timer_first(&context->timers);
    return (timer->deadline > now) ? timer->deadline - now : 0;
}
