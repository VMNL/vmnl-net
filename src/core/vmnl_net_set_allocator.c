#include "core/allocator_internal.h"

#include <stdlib.h>

VmnlNetAllocator vmnl_net_allocator = {malloc, realloc, free};

VmnlNetError vmnl_net_set_allocator(VmnlNetAllocate allocate, VmnlNetReallocate reallocate, VmnlNetDeallocate deallocate)
{
    if (allocate == NULL || reallocate == NULL || deallocate == NULL) {
        return VMNL_NET_EINVAL;
    }
    vmnl_net_allocator = (VmnlNetAllocator) {allocate, reallocate, deallocate};
    return VMNL_NET_SUCCESS;
}
