#include "buffer/buffer_internal.h"
#include "error/error_internal.h"

VmnlNetBuffer *vmnl_net_buffer_flat_create(const struct VmnlNetBufferOps *ops, const size_t capacity, VmnlNetError *error)
{
    if (capacity == 0) {
        vmnl_net_error_set(error, VMNL_NET_EINVAL);
        return NULL;
    }
    return vmnl_net_buffer_contiguous_create(ops, capacity, error);
}
