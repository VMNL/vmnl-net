#include "buffer/buffer_internal.h"
#include "error/error_internal.h"

bool vmnl_net_buffer_view(const VmnlNetBuffer *buffer, VmnlNetBufferView views[2], VmnlNetError *error)
{
    size_t view_size;

    if (buffer == NULL || views == NULL) {
        vmnl_net_error_set(error, VMNL_NET_EINVAL);
        return false;
    }
    view_size = buffer->capacity - buffer->start;
    if (view_size > buffer->size) {
        view_size = buffer->size;
    }
    views[0] = (VmnlNetBufferView) {.data = (buffer->data == NULL) ? NULL : buffer->data + buffer->start, .size = view_size};
    views[1] = (VmnlNetBufferView) {.data = buffer->data, .size = buffer->size - view_size};
    vmnl_net_error_set(error, VMNL_NET_SUCCESS);
    return true;
}
