#include "context/context_internal.h"
#include "timer/timer_internal.h"

bool vmnl_net_context_remaining(const VmnlNetContext *context, uint64_t now, uint64_t *remaining)
{
    const VmnlNetTimer *timer;

    if (vmnl_net_list_is_empty(&context->timers.active) == true) {
        return false;
    }
    timer = VMNL_NET_LIST_FRONT(&context->timers.active, VmnlNetTimer, link);
    *remaining = vmnl_net_timer_remaining(timer, now);
    return true;
}
