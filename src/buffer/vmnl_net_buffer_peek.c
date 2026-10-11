#include "buffer/buffer_internal.h"
#include "error/error_internal.h"

bool vmnl_net_buffer_peek(const VmnlNetBuffer *buffer, void *data, size_t size, VmnlNetError *error)
{
    if (buffer == NULL || (data == NULL && size != 0) || size > buffer->size) {
        vmnl_net_error_set(error, VMNL_NET_EINVAL);
        return false;
    }
    vmnl_net_buffer_copy(buffer, data, size);
    vmnl_net_error_set(error, VMNL_NET_SUCCESS);
    return true;
}
