#include "buffer/buffer_internal.h"
#include "error/error_internal.h"

bool vmnl_net_buffer_ring_shrink(VmnlNetBuffer *buffer, const size_t capacity, VmnlNetError *error)
{
    if (capacity == 0) {
        vmnl_net_error_set(error, VMNL_NET_EINVAL);
        return false;
    }
    return vmnl_net_buffer_ring_resize(buffer, capacity, error);
}
