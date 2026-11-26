#include "buffer/buffer_internal.h"
#include "error/error_internal.h"

static const struct VmnlNetBufferOps vmnl_net_buffer_flat_ops = {
    .create = vmnl_net_buffer_flat_create,
    .clear = vmnl_net_buffer_contiguous_clear,
    .resize = NULL,
    .shrink = NULL,
    .prepare = vmnl_net_buffer_flat_prepare,
    .commit = vmnl_net_buffer_flat_commit,
    .data = vmnl_net_buffer_flat_data,
    .consume = vmnl_net_buffer_flat_consume,
    .destroy = vmnl_net_buffer_contiguous_destroy,
};

static const struct VmnlNetBufferOps vmnl_net_buffer_ring_ops = {
    .create = vmnl_net_buffer_flat_create,
    .clear = vmnl_net_buffer_contiguous_clear,
    .resize = vmnl_net_buffer_ring_resize,
    .shrink = vmnl_net_buffer_ring_shrink,
    .prepare = vmnl_net_buffer_ring_prepare,
    .commit = vmnl_net_buffer_ring_commit,
    .data = vmnl_net_buffer_ring_data,
    .consume = vmnl_net_buffer_ring_consume,
    .destroy = vmnl_net_buffer_contiguous_destroy,
};

static const struct VmnlNetBufferOps vmnl_net_buffer_dynamic_ops = {
    .create = vmnl_net_buffer_contiguous_create,
    .clear = vmnl_net_buffer_contiguous_clear,
    .resize = vmnl_net_buffer_dynamic_resize,
    .shrink = vmnl_net_buffer_dynamic_shrink,
    .prepare = vmnl_net_buffer_dynamic_prepare,
    .commit = vmnl_net_buffer_flat_commit,
    .data = vmnl_net_buffer_flat_data,
    .consume = vmnl_net_buffer_flat_consume,
    .destroy = vmnl_net_buffer_contiguous_destroy,
};

static const struct VmnlNetBufferOps vmnl_net_buffer_chunked_ops = {
    .create = vmnl_net_buffer_chunked_create,
    .clear = vmnl_net_buffer_chunked_clear,
    .resize = vmnl_net_buffer_chunked_resize,
    .shrink = vmnl_net_buffer_chunked_shrink,
    .prepare = vmnl_net_buffer_chunked_prepare,
    .commit = vmnl_net_buffer_chunked_commit,
    .data = vmnl_net_buffer_chunked_data,
    .consume = vmnl_net_buffer_chunked_consume,
    .destroy = vmnl_net_buffer_chunked_destroy,
};

static const struct VmnlNetBufferOps *const vmnl_net_buffer_ops[] = {
    [VMNL_NET_BUFFER_FLAT] = &vmnl_net_buffer_flat_ops,
    [VMNL_NET_BUFFER_RING] = &vmnl_net_buffer_ring_ops,
    [VMNL_NET_BUFFER_DYNAMIC] = &vmnl_net_buffer_dynamic_ops,
    [VMNL_NET_BUFFER_CHUNKED] = &vmnl_net_buffer_chunked_ops,
};

VmnlNetBuffer *vmnl_net_buffer_create(VmnlNetBufferType type, size_t capacity, VmnlNetError *error)
{
    if (type >= sizeof vmnl_net_buffer_ops / sizeof vmnl_net_buffer_ops[0] || vmnl_net_buffer_ops[type] == NULL) {
        vmnl_net_error_set(error, VMNL_NET_EINVAL);
        return NULL;
    }
    return vmnl_net_buffer_ops[type]->create(vmnl_net_buffer_ops[type], capacity, error);
}
