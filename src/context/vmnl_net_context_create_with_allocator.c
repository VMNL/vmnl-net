#include "context/context_internal.h"
#include "error/error_internal.h"

VmnlNetContext *vmnl_net_context_create_with_allocator(const VmnlNetAllocator *allocator, VmnlNetError *error)
{
    VmnlNetContext *context;

    if (allocator == NULL || allocator->allocate == NULL || allocator->reallocate == NULL || allocator->deallocate == NULL) {
        vmnl_net_error_set(error, VMNL_NET_EINVAL);
        return NULL;
    }
    context = allocator->allocate(sizeof *context);
    if (context == NULL) {
        vmnl_net_error_set(error, VMNL_NET_ENOMEM);
        return NULL;
    }
    *context = (VmnlNetContext) {*allocator};
    vmnl_net_error_set(error, VMNL_NET_SUCCESS);
    return context;
}
