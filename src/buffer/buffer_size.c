#include "buffer/buffer_internal.h"

#include <stddef.h>

size_t vmnl_net_buffer_size(const VmnlNetBuffer *buffer)
{
    return buffer->size;
}
