#include "allocator/allocator_internal.h"

#include <stdlib.h>

VmnlNetAllocator vmnl_net_allocator = {
    .allocate = malloc,
    .reallocate = realloc,
    .deallocate = free,
};

void vmnl_net_allocator_set(VmnlNetAllocatorAllocate allocate, VmnlNetAllocatorReallocate reallocate, VmnlNetAllocatorDeallocate deallocate)
{
    vmnl_net_allocator = (VmnlNetAllocator) {.allocate = allocate, .reallocate = reallocate, .deallocate = deallocate};
}
