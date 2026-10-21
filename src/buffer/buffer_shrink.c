#include "buffer/buffer_internal.h"

bool vmnl_net_buffer_shrink(VmnlNetBuffer *buffer, const size_t capacity, VmnlNetError *error)
{
    return buffer->ops->shrink(buffer, capacity, error);
}
