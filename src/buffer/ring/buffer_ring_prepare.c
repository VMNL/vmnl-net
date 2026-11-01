#include "buffer/buffer_internal.h"
#include "error/error_internal.h"

static size_t vmnl_net_buffer_ring_count_views_to_prepare(const VmnlNetBuffer *buffer, size_t offset, size_t nbytes)
{
    return (offset < buffer->capacity && nbytes > buffer->capacity - offset) ? 2 : 1;
}

static void vmnl_net_buffer_ring_prepare_after_wrap(const VmnlNetBuffer *buffer, size_t offset, VmnlNetBufferView *views, size_t *nviews)
{
    views[0].data = ((const struct VmnlNetBufferContiguous *) buffer)->pointer + (offset - buffer->capacity);
    views[0].size = buffer->capacity - buffer->size;
    *nviews = 1;
}

static void vmnl_net_buffer_ring_prepare_across_wrap(const VmnlNetBuffer *buffer, size_t offset, VmnlNetBufferView *views, size_t *nviews)
{
    const struct VmnlNetBufferContiguous *contiguous_buffer = (const struct VmnlNetBufferContiguous *) buffer;

    views[0].data = contiguous_buffer->pointer + offset;
    views[0].size = buffer->capacity - offset;
    if (*nviews == 1 || contiguous_buffer->offset == 0) {
        *nviews = 1;
        return;
    }
    views[1].data = contiguous_buffer->pointer;
    views[1].size = contiguous_buffer->offset;
    *nviews = 2;
}

bool vmnl_net_buffer_ring_prepare(VmnlNetBuffer *buffer, size_t nbytes, VmnlNetBufferView *views, size_t *nviews, VmnlNetError *error)
{
    size_t offset = ((const struct VmnlNetBufferContiguous *) buffer)->offset + buffer->size;

    if (nbytes > buffer->capacity - buffer->size) {
        vmnl_net_error_set(error, VMNL_NET_ENOBUFS);
        return false;
    }
    if (vmnl_net_buffer_ring_count_views_to_prepare(buffer, offset, nbytes) > *nviews) {
        vmnl_net_error_set(error, VMNL_NET_EINVAL);
        return false;
    }
    if (offset >= buffer->capacity) {
        vmnl_net_buffer_ring_prepare_after_wrap(buffer, offset, views, nviews);
    } else {
        vmnl_net_buffer_ring_prepare_across_wrap(buffer, offset, views, nviews);
    }
    vmnl_net_error_set(error, VMNL_NET_SUCCESS);
    return true;
}
