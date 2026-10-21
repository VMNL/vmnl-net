#include "buffer/buffer_internal.h"

bool vmnl_net_buffer_is_empty(const VmnlNetBuffer *buffer)
{
    return buffer->size == 0;
}
