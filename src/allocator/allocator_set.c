#include "allocator/allocator_internal.h"
#include "error/error_internal.h"

#include <stdlib.h>

VmnlNetAllocator vmnl_net_allocator = {
    .allocate = malloc,
    .reallocate = realloc,
    .deallocate = free,
};

bool vmnl_net_allocator_set(void *(*allocate)(size_t size), void *(*reallocate)(void *pointer, size_t size), void (*deallocate)(void *pointer), VmnlNetError *error)
{
    if (allocate == NULL || reallocate == NULL || deallocate == NULL) {
        vmnl_net_error_set(error, VMNL_NET_EINVAL);
        return false;
    }
    vmnl_net_allocator = (VmnlNetAllocator) {.allocate = allocate, .reallocate = reallocate, .deallocate = deallocate};
    vmnl_net_error_set(error, VMNL_NET_SUCCESS);
    return true;
}
