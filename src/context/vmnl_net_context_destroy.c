#include "context/context_internal.h"

void vmnl_net_context_destroy(VmnlNetContext *context)
{
    if (context == NULL) {
        return;
    }
    context->allocator.deallocate(context);
}
