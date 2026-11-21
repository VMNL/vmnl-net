#include "buffer/buffer_internal.h"

#include <stddef.h>

void vmnl_net_buffer_flat_consume(VmnlNetBuffer *buffer, size_t nbytes)
{
    buffer->size -= nbytes;
    ((struct VmnlNetBufferContiguous *) buffer)->offset += nbytes;
    if (buffer->size == 0) {
        ((struct VmnlNetBufferContiguous *) buffer)->offset = 0;
    }
}
