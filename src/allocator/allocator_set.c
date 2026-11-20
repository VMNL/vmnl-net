#include "allocator/allocator_internal.h"

#include <stdlib.h>

static void *vmnl_net_allocator_default_allocator(size_t size)
{
    return malloc(size);
}

static void *vmnl_net_allocator_default_reallocator(void *pointer, size_t size)
{
    return realloc(pointer, size);
}

static void vmnl_net_allocator_default_deallocator(void *pointer)
{
    free(pointer);
}

VmnlNetAllocator vmnl_net_allocator = {
    .allocate = vmnl_net_allocator_default_allocator,
    .reallocate = vmnl_net_allocator_default_reallocator,
    .deallocate = vmnl_net_allocator_default_deallocator,
};

void vmnl_net_allocator_set(VmnlNetAllocatorAllocate allocate, VmnlNetAllocatorReallocate reallocate, VmnlNetAllocatorDeallocate deallocate)
{
    vmnl_net_allocator = (VmnlNetAllocator) {.allocate = allocate, .reallocate = reallocate, .deallocate = deallocate};
}
