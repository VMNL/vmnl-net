#include "buffer/buffer_internal.h"
#include "context/context_internal.h"

void vmnl_net_buffer_destroy(VmnlNetContext *context, VmnlNetBuffer *buffer)
{
    if (context == NULL || buffer == NULL) {
        return;
    }
    vmnl_net_buffer_release(context, buffer);
    context->allocator.deallocate(buffer);
}
