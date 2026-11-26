#include "buffer/buffer_internal.h"
#include "error/error_internal.h"

bool vmnl_net_buffer_ring_prepare(VmnlNetBuffer *buffer, size_t nbytes, VmnlNetBufferView *views, size_t *nviews, VmnlNetError *error)
{
    struct VmnlNetBufferSpan span;
    struct VmnlNetBufferSpan other;

    if (nbytes > buffer->capacity - buffer->size) {
        vmnl_net_error_set(error, VMNL_NET_ENOBUFS);
        return false;
    }
    span = vmnl_net_buffer_ring_spans(buffer, ((const struct VmnlNetBufferContiguous *) buffer)->offset + buffer->size, buffer->capacity - buffer->size, &other);
    if (vmnl_net_buffer_count_views_pair(span.size, nbytes, VMNL_NET_BUFFER_VIEW_SIZE_MAX) > *nviews) {
        vmnl_net_error_set(error, VMNL_NET_EINVAL);
        return false;
    }
    (void) vmnl_net_buffer_describe_pair(span, other, VMNL_NET_BUFFER_VIEW_SIZE_MAX, views, nviews);
    vmnl_net_error_set(error, VMNL_NET_SUCCESS);
    return true;
}
