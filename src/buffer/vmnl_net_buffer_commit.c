#include "buffer/buffer_internal.h"
#include "error/error_internal.h"

bool vmnl_net_buffer_commit(VmnlNetBuffer *buffer, size_t size, VmnlNetError *error)
{
    size_t free_size;

    if (buffer == NULL) {
        vmnl_net_error_set(error, VMNL_NET_EINVAL);
        return false;
    }
    free_size = buffer->capacity - buffer->size;
    if (buffer->type == VMNL_NET_BUFFER_TYPE_DYNAMIC) {
        free_size -= buffer->start;
    }
    if (size > free_size) {
        vmnl_net_error_set(error, VMNL_NET_EINVAL);
        return false;
    }
    buffer->size += size;
    vmnl_net_error_set(error, VMNL_NET_SUCCESS);
    return true;
}
