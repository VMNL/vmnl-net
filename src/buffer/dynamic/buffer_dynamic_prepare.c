#include "buffer/buffer_internal.h"
#include "error/error_internal.h"

#include <string.h>

static size_t vmnl_net_buffer_dynamic_compute_grown_capacity(const VmnlNetBuffer *buffer, size_t nbytes)
{
    size_t grown_capacity = (size_t) ((double) buffer->capacity * VMNL_NET_BUFFER_REALLOCATION_FACTOR);
    size_t needed_capacity = buffer->size + nbytes;
    size_t capacity = (grown_capacity < needed_capacity) ? needed_capacity : grown_capacity;

    return (capacity < VMNL_NET_BUFFER_MINIMUM_CAPACITY) ? VMNL_NET_BUFFER_MINIMUM_CAPACITY : capacity;
}

bool vmnl_net_buffer_dynamic_prepare(VmnlNetBuffer *buffer, size_t nbytes, VmnlNetBufferView *views, size_t *nviews, VmnlNetError *error)
{
    struct VmnlNetBufferContiguous *contiguous_buffer = (struct VmnlNetBufferContiguous *) buffer;

    if (nbytes > SIZE_MAX - buffer->size) {
        vmnl_net_error_set(error, VMNL_NET_ENOMEM);
        return false;
    }
    if (vmnl_net_buffer_count_views(nbytes, VMNL_NET_BUFFER_VIEW_SIZE_MAX) > *nviews) {
        vmnl_net_error_set(error, VMNL_NET_EINVAL);
        return false;
    }
    if (nbytes > buffer->capacity - buffer->size && vmnl_net_buffer_dynamic_resize(buffer, vmnl_net_buffer_dynamic_compute_grown_capacity(buffer, nbytes), error) == false) {
        return false;
    }
    if (nbytes > buffer->capacity - contiguous_buffer->offset - buffer->size) {
        memmove(contiguous_buffer->pointer, contiguous_buffer->pointer + contiguous_buffer->offset, buffer->size);
        contiguous_buffer->offset = 0;
    }
    (void) vmnl_net_buffer_describe(contiguous_buffer->pointer + contiguous_buffer->offset + buffer->size, buffer->capacity - contiguous_buffer->offset - buffer->size, VMNL_NET_BUFFER_VIEW_SIZE_MAX, views, nviews);
    vmnl_net_error_set(error, VMNL_NET_SUCCESS);
    return true;
}
