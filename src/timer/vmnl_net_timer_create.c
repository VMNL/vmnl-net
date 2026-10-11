#include "timer/timer_internal.h"
#include "context/context_internal.h"
#include "error/error_internal.h"

VmnlNetTimer *vmnl_net_timer_create(VmnlNetContext *context, VmnlNetTimerCallback callback, void *user_data, VmnlNetError *error)
{
    VmnlNetTimer *timer;

    if (context == NULL || callback == NULL) {
        vmnl_net_error_set(error, VMNL_NET_EINVAL);
        return NULL;
    }
    timer = context->allocator.allocate(sizeof *timer);
    if (timer == NULL) {
        vmnl_net_error_set(error, VMNL_NET_ENOMEM);
        return NULL;
    }
    *timer = (VmnlNetTimer) {.callback = callback, .user_data = user_data};
    vmnl_net_list_init(&timer->node);
    vmnl_net_error_set(error, VMNL_NET_SUCCESS);
    return timer;
}
