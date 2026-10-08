#include <vmnl/net/context.h>

#include <stdlib.h>

VmnlNetContext *vmnl_net_context_create(VmnlNetError *error)
{
    return vmnl_net_context_create_with_allocator(&(VmnlNetAllocator) {malloc, realloc, free}, error);
}
