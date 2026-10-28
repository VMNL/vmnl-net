#include "buffer/buffer_internal.h"
#include "error/error_internal.h"

bool vmnl_net_buffer_flat_commit(VmnlNetBuffer *buffer, const size_t nbytes, VmnlNetError *error)
{
    if (nbytes > buffer->capacity - ((const struct VmnlNetBufferContiguous *) buffer)->offset - buffer->size) {
        vmnl_net_error_set(error, VMNL_NET_EINVAL);
        return false;
    }
    buffer->size += nbytes;
    vmnl_net_error_set(error, VMNL_NET_SUCCESS);
    return true;
}
