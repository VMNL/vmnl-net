#include "allocator/allocator_internal.h"

#include <stdlib.h>

VmnlNetAllocator vmnl_net_allocator = {
    .allocate = malloc,
    .reallocate = realloc,
    .deallocate = free,
};

void vmnl_net_allocator_set(void *(*allocate)(size_t size), void *(*reallocate)(void *pointer, size_t size), void (*deallocate)(void *pointer))
{
    vmnl_net_allocator = (VmnlNetAllocator) {.allocate = allocate, .reallocate = reallocate, .deallocate = deallocate};
}
