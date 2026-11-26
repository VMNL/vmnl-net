#include "buffer/buffer_internal.h"

#include <stddef.h>

static size_t vmnl_net_buffer_ring_data_after_wrap(const VmnlNetBuffer *buffer, size_t offset, VmnlNetBufferView *views, size_t *nviews)
{
    const struct VmnlNetBufferContiguous *contiguous_buffer = (const struct VmnlNetBufferContiguous *) buffer;

    return vmnl_net_buffer_describe(contiguous_buffer->pointer + (contiguous_buffer->offset + offset - buffer->capacity), buffer->size - offset, views, nviews);
}

static size_t vmnl_net_buffer_ring_data_across_wrap(const VmnlNetBuffer *buffer, size_t offset, VmnlNetBufferView *views, size_t *nviews)
{
    const struct VmnlNetBufferContiguous *contiguous_buffer = (const struct VmnlNetBufferContiguous *) buffer;
    size_t first_nbytes = buffer->capacity - contiguous_buffer->offset - offset;
    size_t first_nviews = *nviews;
    size_t second_nviews;
    size_t nbytes = vmnl_net_buffer_describe(contiguous_buffer->pointer + contiguous_buffer->offset + offset, first_nbytes, views, &first_nviews);

    if (first_nviews == *nviews) {
        *nviews = first_nviews;
        return nbytes;
    }
    second_nviews = *nviews - first_nviews;
    nbytes += vmnl_net_buffer_describe(contiguous_buffer->pointer, buffer->size - offset - first_nbytes, views + first_nviews, &second_nviews);
    *nviews = first_nviews + second_nviews;
    return nbytes;
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
