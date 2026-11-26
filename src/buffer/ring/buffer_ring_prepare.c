#include "buffer/buffer_internal.h"
#include "error/error_internal.h"

#include <stddef.h>

static size_t vmnl_net_buffer_ring_count_views_to_prepare(const VmnlNetBuffer *buffer, size_t offset, size_t nbytes)
{
    size_t first_nbytes = (offset < buffer->capacity) ? buffer->capacity - offset : buffer->capacity - buffer->size;

    return (nbytes <= first_nbytes) ? vmnl_net_buffer_count_views(nbytes) : vmnl_net_buffer_count_views(first_nbytes) + vmnl_net_buffer_count_views(nbytes - first_nbytes);
}

static void vmnl_net_buffer_ring_prepare_after_wrap(const VmnlNetBuffer *buffer, size_t offset, VmnlNetBufferView *views, size_t *nviews)
{
    (void) vmnl_net_buffer_describe(((const struct VmnlNetBufferContiguous *) buffer)->pointer + (offset - buffer->capacity), buffer->capacity - buffer->size, views, nviews);
}

static void vmnl_net_buffer_ring_prepare_across_wrap(const VmnlNetBuffer *buffer, size_t offset, VmnlNetBufferView *views, size_t *nviews)
{
    const struct VmnlNetBufferContiguous *contiguous_buffer = (const struct VmnlNetBufferContiguous *) buffer;
    size_t first_nviews = *nviews;
    size_t second_nviews;

    (void) vmnl_net_buffer_describe(contiguous_buffer->pointer + offset, buffer->capacity - offset, views, &first_nviews);
    if (first_nviews == *nviews || contiguous_buffer->offset == 0) {
        *nviews = first_nviews;
        return;
    }
    second_nviews = *nviews - first_nviews;
    (void) vmnl_net_buffer_describe(contiguous_buffer->pointer, contiguous_buffer->offset, views + first_nviews, &second_nviews);
    *nviews = first_nviews + second_nviews;
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
