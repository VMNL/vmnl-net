#include "buffer/buffer_internal.h"

void vmnl_net_buffer_flat_consume(VmnlNetBuffer *buffer, const size_t nbytes)
{
    buffer->size -= nbytes;
    ((struct VmnlNetBufferContiguous *) buffer)->offset += nbytes;
    if (buffer->size == 0) {
        ((struct VmnlNetBufferContiguous *) buffer)->offset = 0;
    }
}
