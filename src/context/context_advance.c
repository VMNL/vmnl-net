#include "context/context_internal.h"
#include "timer/timer_internal.h"

static void vmnl_net_timer_gather_expired(struct VmnlNetListLink *active_timers, uint64_t now, struct VmnlNetListLink *expired_timers)
{
    VmnlNetTimer *timer;

    VMNL_NET_LIST_FOREACH(timer, active_timers, link)
    {
        if (timer->deadline > now) {
            break;
        }
        vmnl_net_list_remove(&timer->link);
        vmnl_net_list_push_back(expired_timers, &timer->link);
    }
}

static void vmnl_net_timer_call_and_rearm(struct VmnlNetListLink *active_timers, VmnlNetTimer *timer, uint64_t now)
{
    struct VmnlNetListLink honeypot;
    uint64_t lateness = now - timer->deadline;

    vmnl_net_list_init(&honeypot);
    vmnl_net_list_push_back(&honeypot, &timer->link);
    timer->callback(timer, lateness, timer->data);
    if (vmnl_net_list_is_empty(&honeypot) == true) {
        return;
    }
    vmnl_net_list_remove(&timer->link);
    if (timer->interval == 0) {
        return;
    }
    timer->deadline = vmnl_net_timer_compute_deadline(now - lateness % timer->interval, timer->interval);
    vmnl_net_timer_insert(active_timers, timer);
}

void vmnl_net_context_advance(VmnlNetContext *context, uint64_t now)
{
    struct VmnlNetListLink expired_timers;
    VmnlNetTimer *timer;

    vmnl_net_list_init(&expired_timers);
    vmnl_net_timer_gather_expired(&context->timers.active, now, &expired_timers);
    context->timers.now = now;
    while (vmnl_net_list_is_empty(&expired_timers) == false) {
        timer = VMNL_NET_LIST_FRONT(&expired_timers, VmnlNetTimer, link);
        vmnl_net_list_remove(&timer->link);
        if (timer->interval == 0) {
            timer->callback(timer, now - timer->deadline, timer->data);
        } else {
            vmnl_net_timer_call_and_rearm(&context->timers.active, timer, now);
        }
    }
}
