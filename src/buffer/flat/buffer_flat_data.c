#include "buffer/buffer_internal.h"

size_t vmnl_net_buffer_flat_data(const VmnlNetBuffer *buffer, size_t offset, VmnlNetBufferView *views, size_t *nviews)
{
    const struct VmnlNetBufferContiguous *contiguous_buffer = (const struct VmnlNetBufferContiguous *) buffer;

    if (*nviews == 0) {
        *nviews = 0;
        return 0;
    }
    return vmnl_net_buffer_describe(contiguous_buffer->pointer + contiguous_buffer->offset + offset, buffer->size - offset, VMNL_NET_BUFFER_VIEW_SIZE_MAX, views, nviews);
}
