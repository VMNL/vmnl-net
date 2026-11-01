#include <vmnl/net/clock.h>

#include "context/context_internal.h"
#include "allocator/allocator_internal.h"
#include "error/error_internal.h"

VmnlNetContext *vmnl_net_context_create(VmnlNetError *error)
{
    VmnlNetContext *context = vmnl_net_allocator.allocate(sizeof *context);

    if (context == NULL) {
        vmnl_net_error_set(error, VMNL_NET_ENOMEM);
        return NULL;
    }
    *context = (VmnlNetContext) {
        .timers.now = vmnl_net_clock_now(),
    };
    vmnl_net_list_init(&context->timers.active);
    vmnl_net_error_set(error, VMNL_NET_SUCCESS);
    return context;
}
