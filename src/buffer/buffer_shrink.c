#include "buffer/buffer_internal.h"
#include "error/error_internal.h"

bool vmnl_net_buffer_shrink(VmnlNetBuffer *buffer, const size_t capacity, VmnlNetError *error)
{
    if (buffer->ops->shrink == NULL) {
        vmnl_net_error_set(error, VMNL_NET_ENOTSUP);
        return false;
    }
    if (capacity < buffer->size) {
        vmnl_net_error_set(error, VMNL_NET_EINVAL);
        return false;
    }
    if (capacity >= buffer->capacity) {
        vmnl_net_error_set(error, VMNL_NET_SUCCESS);
        return true;
    }
    return buffer->ops->shrink(buffer, capacity, error);
}
