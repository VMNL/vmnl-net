#include "buffer/buffer_internal.h"

bool vmnl_net_buffer_consume(VmnlNetBuffer *buffer, const size_t nbytes, VmnlNetError *error)
{
    return buffer->ops->consume(buffer, nbytes, error);
}
