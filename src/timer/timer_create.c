#include "timer/timer_internal.h"
#include "allocator/allocator_internal.h"
#include "error/error_internal.h"

VmnlNetTimer *vmnl_net_timer_create(VmnlNetTimerCallback callback, void *data, VmnlNetError *error)
{
    VmnlNetTimer *timer = vmnl_net_allocator.allocate(sizeof *timer);

    if (timer == NULL) {
        vmnl_net_error_set(error, VMNL_NET_ENOMEM);
        return NULL;
    }
    *timer = (VmnlNetTimer) {
        .callback = callback,
        .data = data,
    };
    vmnl_net_list_init(&timer->link);
    vmnl_net_error_set(error, VMNL_NET_SUCCESS);
    return timer;
}
