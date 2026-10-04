#include "context/context_internal.h"
#include "core/allocator_internal.h"

VmnlNetError vmnl_net_context_create(VmnlNetContext **context)
{
    if (context == NULL) {
        return VMNL_NET_EINVAL;
    }
    *context = vmnl_net_allocator.allocate(sizeof **context);
    if (*context == NULL) {
        return VMNL_NET_ENOMEM;
    }
    **context = (VmnlNetContext) {0};
    return VMNL_NET_SUCCESS;
}
