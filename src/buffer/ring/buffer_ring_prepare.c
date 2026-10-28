#include "buffer/buffer_internal.h"
#include "error/error_internal.h"

bool vmnl_net_buffer_ring_prepare(VmnlNetBuffer *buffer, const size_t nbytes, VmnlNetBufferView *views, size_t *nviews, VmnlNetError *error)
{
    const struct VmnlNetBufferContiguous *contiguous_buffer = (const struct VmnlNetBufferContiguous *) buffer;
    const size_t offset = contiguous_buffer->offset + buffer->size;

    if (nbytes > buffer->capacity - buffer->size) {
        vmnl_net_error_set(error, VMNL_NET_ENOBUFS);
        return false;
    }
    if (*nviews == 0 || (*nviews == 1 && offset < buffer->capacity && nbytes > buffer->capacity - offset)) {
        vmnl_net_error_set(error, VMNL_NET_EINVAL);
        return false;
    }
    vmnl_net_error_set(error, VMNL_NET_SUCCESS);
    if (offset >= buffer->capacity) {
        views[0].data = contiguous_buffer->pointer + (offset - buffer->capacity);
        views[0].size = buffer->capacity - buffer->size;
        *nviews = 1;
        return true;
    }
    views[0].data = contiguous_buffer->pointer + offset;
    views[0].size = buffer->capacity - offset;
    if (*nviews == 1 || contiguous_buffer->offset == 0) {
        *nviews = 1;
        return true;
    }
    views[1].data = contiguous_buffer->pointer;
    views[1].size = contiguous_buffer->offset;
    *nviews = 2;
    return true;
}
