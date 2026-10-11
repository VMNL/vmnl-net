#include "buffer/buffer_internal.h"
#include "error/error_internal.h"

VmnlNetBuffer *vmnl_net_buffer_create_ring(VmnlNetContext *context, size_t capacity, VmnlNetError *error)
{
    if (context == NULL || capacity == 0) {
        vmnl_net_error_set(error, VMNL_NET_EINVAL);
        return NULL;
    }
    return vmnl_net_buffer_create_generic(context, VMNL_NET_BUFFER_TYPE_RING, capacity, error);
}
