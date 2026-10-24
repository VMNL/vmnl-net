#include "buffer/buffer_internal.h"
#include "error/error_internal.h"

static const struct VmnlNetBufferOps vmnl_net_buffer_flat_ops = {
    .create = vmnl_net_buffer_flat_create,
    .clear = vmnl_net_buffer_contiguous_clear,
    .resize = NULL,
    .shrink = NULL,
    .data = vmnl_net_buffer_flat_data,
    .consume = vmnl_net_buffer_flat_consume,
    .prepare = vmnl_net_buffer_flat_prepare,
    .commit = vmnl_net_buffer_flat_commit,
    .destroy = vmnl_net_buffer_contiguous_destroy,
};

static const struct VmnlNetBufferOps vmnl_net_buffer_ring_ops = {
    .create = vmnl_net_buffer_flat_create,
    .clear = vmnl_net_buffer_contiguous_clear,
    .resize = vmnl_net_buffer_ring_resize,
    .shrink = vmnl_net_buffer_ring_shrink,
    .data = vmnl_net_buffer_ring_data,
    .consume = vmnl_net_buffer_ring_consume,
    .prepare = vmnl_net_buffer_ring_prepare,
    .commit = vmnl_net_buffer_ring_commit,
    .destroy = vmnl_net_buffer_contiguous_destroy,
};

static const struct VmnlNetBufferOps vmnl_net_buffer_dynamic_ops = {
    .create = vmnl_net_buffer_contiguous_create,
    .clear = vmnl_net_buffer_contiguous_clear,
    .resize = vmnl_net_buffer_dynamic_resize,
    .shrink = vmnl_net_buffer_dynamic_shrink,
    .data = vmnl_net_buffer_flat_data,
    .consume = vmnl_net_buffer_flat_consume,
    .prepare = vmnl_net_buffer_dynamic_prepare,
    .commit = vmnl_net_buffer_flat_commit,
    .destroy = vmnl_net_buffer_contiguous_destroy,
};

static const struct VmnlNetBufferOps vmnl_net_buffer_chunks_ops = {
    .create = vmnl_net_buffer_chunked_create,
    .clear = vmnl_net_buffer_chunked_clear,
    .resize = vmnl_net_buffer_chunked_resize,
    .shrink = vmnl_net_buffer_chunked_shrink,
    .data = vmnl_net_buffer_chunked_data,
    .consume = vmnl_net_buffer_chunked_consume,
    .prepare = vmnl_net_buffer_chunked_prepare,
    .commit = vmnl_net_buffer_chunked_commit,
    .destroy = vmnl_net_buffer_chunked_destroy,
};

static const struct VmnlNetBufferOps *const vmnl_net_buffer_ops[] = {
    [VMNL_NET_BUFFER_FLAT] = &vmnl_net_buffer_flat_ops,
    [VMNL_NET_BUFFER_RING] = &vmnl_net_buffer_ring_ops,
    [VMNL_NET_BUFFER_DYNAMIC] = &vmnl_net_buffer_dynamic_ops,
    [VMNL_NET_BUFFER_CHUNKS] = &vmnl_net_buffer_chunks_ops,
};

VmnlNetBuffer *vmnl_net_buffer_create(const VmnlNetBufferType type, const size_t capacity, VmnlNetError *error)
{
    if (type < VMNL_NET_BUFFER_FLAT || type > VMNL_NET_BUFFER_CHUNKS) {
        vmnl_net_error_set(error, VMNL_NET_EINVAL);
        return NULL;
    }
    return vmnl_net_buffer_ops[type]->create(vmnl_net_buffer_ops[type], capacity, error);
}
