#include "buffer/buffer_internal.h"
#include "error/error_internal.h"

static const struct VmnlNetBufferOps vmnl_net_buffer_ring_ops = {
    .clear      = NULL,
    .resize     = NULL,
    .shrink     = NULL,
    .data       = NULL,
    .consume    = NULL,
    .prepare    = NULL,
    .commit     = NULL,
    .destroy    = NULL,
};

VmnlNetBuffer *vmnl_net_buffer_create_ring(const size_t capacity, VmnlNetError *error)
{
    if (capacity == 0) {
        vmnl_net_error_set(error, VMNL_NET_EINVAL);
        return NULL;       
    }
    return vmnl_net_buffer_create_contiguous(&vmnl_net_buffer_ring_ops, capacity, error);
}
