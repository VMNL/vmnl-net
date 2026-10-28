#include "buffer/buffer_internal.h"
#include "error/error_internal.h"

bool vmnl_net_buffer_commit(VmnlNetBuffer *buffer, const size_t nbytes, VmnlNetError *error)
{
    return buffer->ops->commit(buffer, nbytes, error);
}
