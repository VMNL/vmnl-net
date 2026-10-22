#include "buffer/buffer_internal.h"

void vmnl_net_buffer_contiguous_consume(VmnlNetBuffer *buffer, const size_t nbytes)
{
    buffer->size -= nbytes;
    ((struct VmnlNetBufferContiguous *) buffer)->offset = (((const struct VmnlNetBufferContiguous *) buffer)->offset + nbytes) % buffer->capacity;
}
