#include "allocator/allocator_internal.h"
#include "context/context_internal.h"
#include "error/error_internal.h"

VmnlNetContext *vmnl_net_context_create(VmnlNetError *error)
{
    VmnlNetContext *context = vmnl_net_allocator.allocate(sizeof *context);

    if (context == NULL) {
        vmnl_net_error_set(error, VMNL_NET_ENOMEM);
        return NULL;
    }
    *context = (VmnlNetContext) {0};
    vmnl_net_list_init(&context->timers);
    vmnl_net_error_set(error, VMNL_NET_SUCCESS);
    return context;
}
