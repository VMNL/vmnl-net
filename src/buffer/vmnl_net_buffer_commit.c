#include "buffer/buffer_internal.h"

void vmnl_net_buffer_commit(VmnlNetBuffer *buffer, size_t size)
{
    buffer->size += size;
}
