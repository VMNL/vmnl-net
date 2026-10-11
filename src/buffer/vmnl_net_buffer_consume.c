#include "buffer/buffer_internal.h"
#include "error/error_internal.h"

bool vmnl_net_buffer_consume(VmnlNetBuffer *buffer, size_t size, VmnlNetError *error)
{
    if (buffer == NULL || size > buffer->size) {
        vmnl_net_error_set(error, VMNL_NET_EINVAL);
        return false;
    }
    vmnl_net_buffer_discard(buffer, size);
    vmnl_net_error_set(error, VMNL_NET_SUCCESS);
    return true;
}
