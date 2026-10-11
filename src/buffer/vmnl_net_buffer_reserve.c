#include "buffer/buffer_internal.h"
#include "error/error_internal.h"

bool vmnl_net_buffer_reserve(VmnlNetContext *context, VmnlNetBuffer *buffer, size_t capacity, VmnlNetError *error)
{
    if (context == NULL || buffer == NULL) {
        vmnl_net_error_set(error, VMNL_NET_EINVAL);
        return false;
    }
    if (capacity <= buffer->capacity) {
        vmnl_net_error_set(error, VMNL_NET_SUCCESS);
        return true;
    }
    if (buffer->type == VMNL_NET_BUFFER_TYPE_RING) {
        vmnl_net_error_set(error, VMNL_NET_ENOBUFS);
        return false;
    }
    return vmnl_net_buffer_resize(context, buffer, capacity, error);
}
