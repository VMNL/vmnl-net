#include "buffer/buffer_internal.h"

static size_t vmnl_net_buffer_ring_data_after_wrap(const VmnlNetBuffer *buffer, size_t offset, VmnlNetBufferView *views, size_t *nviews)
{
    const struct VmnlNetBufferContiguous *contiguous_buffer = (const struct VmnlNetBufferContiguous *) buffer;

    views[0].data = contiguous_buffer->pointer + (contiguous_buffer->offset + offset - buffer->capacity);
    views[0].size = buffer->size - offset;
    *nviews = 1;
    return views[0].size;
}

static size_t vmnl_net_buffer_ring_data_across_wrap(const VmnlNetBuffer *buffer, size_t offset, VmnlNetBufferView *views, size_t *nviews)
{
    const struct VmnlNetBufferContiguous *contiguous_buffer = (const struct VmnlNetBufferContiguous *) buffer;

    views[0].data = contiguous_buffer->pointer + contiguous_buffer->offset + offset;
    views[0].size = buffer->capacity - contiguous_buffer->offset - offset;
    if (*nviews == 1) {
        return views[0].size;
    }
    views[1].data = contiguous_buffer->pointer;
    views[1].size = buffer->size - offset - views[0].size;
    *nviews = 2;
    return buffer->size - offset;
}

size_t vmnl_net_buffer_ring_data(const VmnlNetBuffer *buffer, size_t offset, VmnlNetBufferView *views, size_t *nviews)
{
    const struct VmnlNetBufferContiguous *contiguous_buffer = (const struct VmnlNetBufferContiguous *) buffer;

    if (contiguous_buffer->offset + buffer->size <= buffer->capacity || *nviews == 0) {
        return vmnl_net_buffer_flat_data(buffer, offset, views, nviews);
    }
    if (contiguous_buffer->offset + offset >= buffer->capacity) {
        return vmnl_net_buffer_ring_data_after_wrap(buffer, offset, views, nviews);
    }
    return vmnl_net_buffer_ring_data_across_wrap(buffer, offset, views, nviews);
}
