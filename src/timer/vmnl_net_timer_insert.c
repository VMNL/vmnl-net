#include "timer/timer_internal.h"
#include "context/context_internal.h"

void vmnl_net_timer_insert(VmnlNetContext *context, VmnlNetTimer *timer)
{
    VmnlNetList *position = &context->timers;

    while (position->prev != &context->timers && VMNL_NET_LIST_ENTRY(position->prev, VmnlNetTimer, node)->deadline > timer->deadline) {
        position = position->prev;
    }
    vmnl_net_list_insert(&timer->node, position);
}
