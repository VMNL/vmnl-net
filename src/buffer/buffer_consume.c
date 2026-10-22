#include "buffer/buffer_internal.h"
#include "error/error_internal.h"

bool vmnl_net_buffer_consume(VmnlNetBuffer *buffer, const size_t nbytes, VmnlNetError *error)
{
    if (nbytes > buffer->size) {
        vmnl_net_error_set(error, VMNL_NET_EINVAL);
        return false;
    }
    if (buffer->capacity > 0) {
        buffer->ops->consume(buffer, nbytes);
    }
    vmnl_net_error_set(error, VMNL_NET_SUCCESS);
    return true;
}
