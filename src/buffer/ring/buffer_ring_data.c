#include "buffer/buffer_internal.h"

size_t vmnl_net_buffer_ring_data(const VmnlNetBuffer *buffer, const size_t offset, VmnlNetBufferView *views, size_t *nviews)
{
    const struct VmnlNetBufferContiguous *contiguous_buffer = (const struct VmnlNetBufferContiguous *) buffer;
    const size_t nbytes = buffer->size - offset;
    const size_t wrap_offset = buffer->capacity - contiguous_buffer->offset;

    if (contiguous_buffer->offset + buffer->size <= buffer->capacity || *nviews == 0) {
        return vmnl_net_buffer_flat_data(buffer, offset, views, nviews);
    }
    if (offset >= wrap_offset) {
        views[0] = (VmnlNetBufferView) {
            .data = contiguous_buffer->pointer + (offset - wrap_offset),
            .size = nbytes,
        };
        *nviews = 1;
        return nbytes;
    }
    views[0] = (VmnlNetBufferView) {
        .data = contiguous_buffer->pointer + contiguous_buffer->offset + offset,
        .size = wrap_offset - offset,
    };
    if (*nviews == 1) {
        return views[0].size;
    }
    views[1] = (VmnlNetBufferView) {
        .data = contiguous_buffer->pointer,
        .size = nbytes - views[0].size,
    };
    *nviews = 2;
    return nbytes;
}
