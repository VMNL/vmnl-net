#include "buffer/buffer_internal.h"

size_t vmnl_net_buffer_capacity(const VmnlNetBuffer *buffer)
{
    return (buffer == NULL) ? 0 : buffer->capacity;
}
