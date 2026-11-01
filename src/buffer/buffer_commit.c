#include "buffer/buffer_internal.h"
#include "error/error_internal.h"

bool vmnl_net_buffer_commit(VmnlNetBuffer *buffer, size_t nbytes, VmnlNetError *error)
{
    if (nbytes == 0) {
        vmnl_net_error_set(error, VMNL_NET_SUCCESS);
        return true;
    }
    return buffer->ops->commit(buffer, nbytes, error);
}
