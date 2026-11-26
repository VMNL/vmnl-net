#include "buffer/buffer_internal.h"

size_t vmnl_net_buffer_ring_data(const VmnlNetBuffer *buffer, size_t offset, VmnlNetBufferView *views, size_t *nviews)
{
    struct VmnlNetBufferSpan span;
    struct VmnlNetBufferSpan other;

    if (*nviews == 0) {
        return 0;
    }
    span = vmnl_net_buffer_ring_spans(buffer, ((const struct VmnlNetBufferContiguous *) buffer)->offset + offset, buffer->size - offset, &other);
    return vmnl_net_buffer_describe_pair(span, other, VMNL_NET_BUFFER_VIEW_SIZE_MAX, views, nviews);
}
