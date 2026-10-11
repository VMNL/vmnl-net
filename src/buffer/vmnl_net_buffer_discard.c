#include "buffer/buffer_internal.h"

void vmnl_net_buffer_discard(VmnlNetBuffer *buffer, size_t size)
{
    buffer->start += size;
    buffer->size -= size;
    if (buffer->start >= buffer->capacity) {
        buffer->start -= buffer->capacity;
    }
    if (buffer->size == 0) {
        buffer->start = 0;
    }
}
