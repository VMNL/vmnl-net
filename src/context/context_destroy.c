#include "allocator/allocator_internal.h"
#include "context/context_internal.h"

void vmnl_net_context_destroy(VmnlNetContext *context)
{
    if (context == NULL) {
        return;
    }
    vmnl_net_allocator.deallocate(context);
}
