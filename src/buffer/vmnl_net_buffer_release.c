#include "buffer/buffer_internal.h"
#include "context/context_internal.h"

void vmnl_net_buffer_release(VmnlNetContext *context, VmnlNetBuffer *buffer)
{
    context->allocator.deallocate(buffer->data);
    *buffer = (VmnlNetBuffer) {.type = buffer->type};
}
