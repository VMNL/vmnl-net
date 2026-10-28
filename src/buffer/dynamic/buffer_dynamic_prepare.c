#include "buffer/buffer_internal.h"
#include "error/error_internal.h"

#include <string.h>

bool vmnl_net_buffer_dynamic_prepare(VmnlNetBuffer *buffer, const size_t nbytes, VmnlNetBufferView *views, size_t *nviews, VmnlNetError *error)
{
    struct VmnlNetBufferContiguous *contiguous_buffer = (struct VmnlNetBufferContiguous *) buffer;
    size_t capacity = (size_t) ((double) buffer->capacity * VMNL_NET_BUFFER_REALLOCATION_FACTOR);

    if (*nviews == 0) {
        vmnl_net_error_set(error, VMNL_NET_EINVAL);
        return false;
    }
    if (nbytes > SIZE_MAX - buffer->size) {
        vmnl_net_error_set(error, VMNL_NET_ENOMEM);
        return false;
    }
    capacity = (capacity < buffer->size + nbytes) ? buffer->size + nbytes : capacity;
    capacity = (capacity < VMNL_NET_BUFFER_MINIMUM_CAPACITY) ? VMNL_NET_BUFFER_MINIMUM_CAPACITY : capacity;
    if (nbytes > buffer->capacity - buffer->size && vmnl_net_buffer_dynamic_resize(buffer, capacity, error) == false) {
        return false;
    }
    if (nbytes > buffer->capacity - contiguous_buffer->offset - buffer->size) {
        memmove(contiguous_buffer->pointer, contiguous_buffer->pointer + contiguous_buffer->offset, buffer->size);
        contiguous_buffer->offset = 0;
    }
    views[0].data = contiguous_buffer->pointer + contiguous_buffer->offset + buffer->size;
    views[0].size = buffer->capacity - contiguous_buffer->offset - buffer->size;
    *nviews = 1;
    vmnl_net_error_set(error, VMNL_NET_SUCCESS);
    return true;
}
