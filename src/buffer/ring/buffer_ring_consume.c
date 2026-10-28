#include "buffer/buffer_internal.h"

void vmnl_net_buffer_ring_consume(VmnlNetBuffer *buffer, const size_t nbytes)
{
    vmnl_net_buffer_flat_consume(buffer, nbytes);
    if (((const struct VmnlNetBufferContiguous *) buffer)->offset >= buffer->capacity) {
        ((struct VmnlNetBufferContiguous *) buffer)->offset -= buffer->capacity;
    }
}
