#include "context/context_internal.h"
#include "timer/timer_internal.h"

uint64_t vmnl_net_context_remaining(const VmnlNetContext *context, uint64_t now)
{
    if (vmnl_net_list_is_empty(&context->timers.active) == true) {
        return UINT64_MAX;
    }
    return vmnl_net_timer_remaining(VMNL_NET_LIST_FRONT(&context->timers.active, VmnlNetTimer, link), now);
}
