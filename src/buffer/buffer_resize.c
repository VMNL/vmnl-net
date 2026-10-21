#include "buffer/buffer_internal.h"

bool vmnl_net_buffer_resize(VmnlNetBuffer *buffer, const size_t capacity, VmnlNetError *error)
{
    return buffer->ops->resize(buffer, capacity, error);
}
