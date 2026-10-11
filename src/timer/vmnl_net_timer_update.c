#include "timer/timer_internal.h"
#include "context/context_internal.h"

void vmnl_net_timer_update(VmnlNetContext *context, uint64_t now)
{
    VmnlNetList expired;
    VmnlNetTimer *timer;

    if (context == NULL) {
        return;
    }
    vmnl_net_list_init(&expired);
    for (timer = vmnl_net_timer_first(&context->timers); timer != NULL && timer->deadline <= now; timer = vmnl_net_timer_first(&context->timers)) {
        vmnl_net_timer_remove(timer);
        vmnl_net_list_insert(&timer->node, &expired);
    }
    for (timer = vmnl_net_timer_first(&expired); timer != NULL; timer = vmnl_net_timer_first(&expired)) {
        vmnl_net_timer_remove(timer);
        if (timer->interval != 0) {
            timer->deadline = vmnl_net_timer_add(timer->deadline, timer->interval);
            if (timer->deadline <= now) {
                timer->deadline = vmnl_net_timer_add(now, timer->interval);
            }
            vmnl_net_timer_insert(context, timer);
        }
        timer->callback(timer, timer->user_data);
    }
}
