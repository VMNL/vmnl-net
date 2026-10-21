#include "buffer/buffer_internal.h"

static const struct VmnlNetBufferOps vmnl_net_buffer_dynamic_ops = {
    .clear      = NULL,
    .resize     = NULL,
    .shrink     = NULL,
    .data       = NULL,
    .consume    = NULL,
    .prepare    = NULL,
    .commit     = NULL,
    .destroy    = NULL,
};

VmnlNetBuffer *vmnl_net_buffer_create_dynamic(const size_t capacity, VmnlNetError *error)
{
    return vmnl_net_buffer_create_contiguous(&vmnl_net_buffer_dynamic_ops, capacity, error);
}
